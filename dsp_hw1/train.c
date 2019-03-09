#include"hmm.h"
#include<math.h>
#include<stdio.h>
#include<string.h>

void seq_to_state(char seq[]){
	//printf("%s", seq);
	for(int i=0 ; i<strlen(seq)-1 ; i++){
		seq[i] -= 17;
	}
}

// ./train iteration model_init.txt seq_model_01.txt model_01.txt
int main(int argc, char** argv){
	HMM hmm_initial;
	loadHMM( &hmm_initial, argv[2] );
	dumpHMM( stderr, &hmm_initial );
	//printf("start iter\n");
	for(int iter=0 ; iter < atoi(argv[1]) ; iter++){
		FILE *fp = open_or_die(argv[3], "r");
		char seq[100] ;
		memset(seq, '\0' , sizeof(seq));
		// read a sequence
		int N = 0 ;
		
		double gamma_observe[hmm_initial.observ_num][hmm_initial.state_num]  ;
		memset(gamma_observe, 0 , sizeof(gamma_observe));
		double gamma[60][hmm_initial.state_num]  ;
		memset(gamma, 0 , sizeof(gamma));
		double epslong[60][hmm_initial.state_num][hmm_initial.state_num]  ;
		memset(epslong, 0 , sizeof(epslong));
		double epslong_con[hmm_initial.state_num][hmm_initial.state_num]  ;
		memset(epslong_con, 0 , sizeof(epslong_con));
	//	printf("catch char\n");
		double epslong_sum[hmm_initial.state_num][hmm_initial.state_num] ;
		
		double gamma_tail[hmm_initial.state_num] ;
		while(fgets(seq,100,fp)){
			N++;
		//	printf("%s\n",seq);
			//printf("%d\n",strlen(seq));
			seq_to_state(seq);		// transform to number	
		//	printf("%s\n",seq);
			//compute alpha
		//	printf("%d\n",seq[0]);
			double alpha[strlen(seq)-1][hmm_initial.state_num]  ;
			for(int j=0 ; j<hmm_initial.state_num ; j++){
				int seq_num = seq[0] - 48 ;
				alpha[0][j] = hmm_initial.initial[j]*hmm_initial.observation[seq_num][j];
			//	printf("%lf\n",alpha[0][j]);
			}
			for(int pos=1 ; pos<strlen(seq)-1 ; pos++ ){
				for(int j=0 ; j<hmm_initial.state_num ; j++){
					// compute the sum
					double forword_num = 0;
					for(int k=0 ; k<hmm_initial.state_num ; k++){
						forword_num += (alpha[pos-1][k] * hmm_initial.transition[k][j]);
						
					}
					int seq_num = seq[pos] - 48 ;
					forword_num *= hmm_initial.observation[seq_num][j];
					alpha[pos][j] = forword_num;
				}
			}
			/*
			printf("alpha end\n");
			for(int j=0 ; j<strlen(seq) ; j++){
				for(int k=0 ; k<hmm_initial.state_num ; k++){
					printf("%lf ",alpha[j][k]);
				}
				printf("\n");
			}
			*/
			//compute beta
			double beta[strlen(seq)-1][hmm_initial.state_num]  ;
			for(int j=0 ; j<hmm_initial.state_num ; j++){
				beta[strlen(seq)-2][j] = 1 ;
			}
			for(int pos=strlen(seq)-3 ; pos >=0 ; pos--){
				for(int j=0 ; j<hmm_initial.state_num ; j++){
					double backward_num = 0;
					for(int k=0 ; k<hmm_initial.state_num ; k++){
						int seq_num = seq[pos+1] - 48 ;
						
						backward_num += hmm_initial.transition[j][k] * hmm_initial.observation[seq_num][k] * beta[pos+1][k]; 
					}
				
					beta[pos][j] = backward_num ;
				}
				
			}
			/*
			printf("beta end\n");
			for(int j=0 ; j<strlen(seq)-1 ; j++){
				for(int k=0 ; k<hmm_initial.state_num ; k++){
					printf("%lf ",beta[j][k]);
				}
				printf("\n");
			}
			*/
			//compute gamma

			for(int j=0 ; j<strlen(seq)-1 ; j++){
				double mutiple = 0;
				for(int k=0 ; k<hmm_initial.state_num ; k++){		
					mutiple += (alpha[j][k] * beta[j][k]);	
				} 
				for(int k=0 ; k<hmm_initial.state_num ; k++){
					int seq_num = seq[j] - 48 ;
				//	printf("%d %d %e %e %e\n",seq_num,j,mutiple,alpha[j][k],beta[j][k]);
					gamma[j][k] += (alpha[j][k] * beta[j][k] / mutiple) ; // in sequence
				//	printf("%lf\n",gamma[j][k]);
					gamma_observe[seq_num][k] += (alpha[j][k] * beta[j][k] / mutiple); // in observe
				//	printf("%lf\n",gamma_observe[seq_num][k]);
				}
			}
			
		//	printf("gamma end\n");
			
			
			
			// compute epslong
				
				
			// compute epslong_sum (A)
			
			/* 
			for (int j = 0; j < hmm_initial.state_num; j++){
				for (int k = 0; k < hmm_initial.state_num; k++){
					double sum = 0 ;
					for(int pos = 0 ; pos<strlen(seq)-1 ; pos++){
						sum += epslong[pos][j][k];
						epslong_sum[j][k] += sum ;
					}
				}
				
				gamma_tail[j] += gamma[strlen(seq)-1][j];
			}
   */ 
    
			
			
			
			for(int pos=0 ; pos<strlen(seq)-1 ; pos++){
				double sum = 0;
				for(int k=0 ; k<hmm_initial.state_num ; k++){		
					sum += (alpha[pos][k] * beta[pos][k]);	
				}
				for(int j=0 ; j<hmm_initial.state_num ; j++){
					if(pos!=strlen(seq)-2){
						for(int k=0 ; k<hmm_initial.state_num ; k++){
							int seq_num = seq[pos+1] - 48 ;
							epslong_con[j][k] += alpha[pos][j]*hmm_initial.transition[j][k]*hmm_initial.observation[seq_num][k]*beta[pos+1][k]/sum;
						}	
					}
				} 
			} 
			 
			 
			
		//	printf("epilong end\n");
		}	
		
		// update pi
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			hmm_initial.initial[j] = gamma[0][j]/N; 
		}
		
		// update A
		
		// epslong_sum
		/*
		double epslong_sum[hmm_initial.state_num][hmm_initial.state_num]  ;
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			for(int k=0 ; k<hmm_initial.state_num ; k++){	
				epslong_sum[j][k] += epslong_con[j][k];	
			}
		}
		*/
		// gamma_sum
		double gamma_sum[hmm_initial.state_num] ;
		memset(gamma_sum, 0 , sizeof(gamma_sum));
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			for(int pos=0 ; pos < strlen(seq) ; pos++){
				int seq_num = seq[pos] - 48 ;
				gamma_sum[j] += gamma[pos][j] ;
			}
		}
		
		// A
		
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			for(int k=0 ; k<hmm_initial.state_num ; k++){
				hmm_initial.transition[j][k] = epslong_con[j][k] / gamma_sum[j] ;
			}
		}
		
		
		/* 
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			double sum = 0 ;
			for(int k=0 ; k<hmm_initial.observ_num ; k++){
				sum += gamma_sum[j][k] ;
			}
			sum -= gamma_tail[j] ;
			for(int k=0 ; k<hmm_initial.state_num ; k++){
				hmm_initial.transition[j][k] = epslong_sum[j][k] / sum ;
			}
		}
		*/ 
		
		// update B
		
	
		for(int j=0 ; j<hmm_initial.state_num ; j++){
			double sum = 0 ;
			for(int ob=0 ; ob < hmm_initial.observ_num ; ob++){
				sum += gamma_observe[ob][j] ;
			}
			for(int ob=0 ; ob < hmm_initial.observ_num ; ob++){
				hmm_initial.observation[ob][j] = gamma_observe[ob][j] / sum;
			}
		}
		
		/*
		for(int ob=0 ; ob < hmm_initial.observ_num ; ob++){
			for(int j=0 ; j<hmm_initial.state_num ; j++){
				hmm_initial.observation[ob][j] = gamma_observe[ob][j] / gamma_sum[j];
			}
		}*/
		
		
		
		fclose(fp);
		printf("iter times = %d\n",iter);		
	}
	
	FILE *fp = open_or_die(argv[4], "w");
	dumpHMM(fp , &hmm_initial);
	fclose(fp);
	
		
}
