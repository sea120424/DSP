#include"hmm.h"
#include<math.h>
#include<string.h>
void seq_to_state(char seq[]){
	//printf("%s", seq);
	for(int i=0 ; i<strlen(seq)-1 ; i++){
		seq[i] -= 17;
	}
}

int ctoi(char c){
	return (c - 48) ;
}


int times = 1 ;
// ./test modellist.txt testing_data.txt result.txt
int main(int argc, char** argv)
{

	HMM hmms[5];
	load_models(argv[1], hmms, 5);
	char file_name[5][40] ;
	FILE *fp = open_or_die(argv[1],"r");
	for(int i=0 ; i<5 ; i++){
		fscanf(fp,"%s",file_name[i]);
	//	file_name[i][strlen(file_name[i])-1] = '\0' ;
	//	printf("%s",file_name[i]);
	}
	fclose(fp);
	
	FILE *fin = open_or_die(argv[2],"r");
	FILE *fout = fopen(argv[3],"w");
	
	char seq[100] ;
	memset(seq, '\0' , sizeof(seq));
	while(fgets(seq,100,fin)){
		
		seq_to_state(seq);
		if(times){
			printf("%s %d\n",seq,strlen(seq));
			times--;
		}
		double max_prop = 0 ;
		int max_index = 0 ;
		for(int count=0 ; count<5 ; count++){
			double delta[strlen(seq)][hmms[count].state_num] ;
			memset(delta, 0 , sizeof(delta));
			// initial
			for(int i=0 ; i<hmms[count].state_num ; i++){
				delta[0][i] = hmms[count].initial[i] * hmms[count].observation[ctoi(seq[0])][i];
			//	printf("%lf\n",delta[0][i]);
			}
			for(int next=1 ; next<strlen(seq)-1 ; next++){
				double max_chance = 0 ;
			//	max_item = 0 ;
				for(int j=0 ; j<hmms[count].state_num ; j++){
					
					for(int i=0 ; i<hmms[count].state_num ; i++){
						double temp_chance = (delta[next-1][i] * hmms[count].transition[i][j]) ;
						if(temp_chance > max_chance){
							max_chance = temp_chance;
							//max_item = 
						}
					}
				//	printf("---------------->  %d\n",ctoi(seq[next]));
					delta[next][j] = max_chance * hmms[count].observation[ctoi(seq[next])][j];
				//	printf("%lf\n",delta[next][j]);
				}
			}
			
			for(int i=0 ; i<hmms[count].state_num ; i++){
				if(delta[strlen(seq)-2][i] > max_prop){
					max_prop = delta[strlen(seq)-2][i];
					max_index = count ;
				//	printf(">>>>>>>> %lf %d\n",max_prop,max_index);
				}
			}
		}
		//fputs(file_name[max_index],fout);
		//fprintf(fout, "%s",file_name[max_index]);
		
		//fwrite(file_name[max_index],strlen(file_name[max_index])+1,1,fout);
	//	fprintf(fout, "%s\t%e\n",file_name[max_index],max_prop);
		//fprintf(fout, "%s\n",file_name[max_index]);
		fprintf(fout, "%s\t%e\n",file_name[max_index],max_prop);
		
		memset(seq, '\0' , sizeof(seq));
	}
	
	fclose(fin);
	fclose(fout);
	
	//dump_models( hmms, 5);
	
	/*
	HMM hmm_initial;
	loadHMM( &hmm_initial, "model_init.txt" );
	dumpHMM( stderr, &hmm_initial );
	*/
	printf("%f\n", log(1.5) ); // make sure the math library is included
	return 0;
}
