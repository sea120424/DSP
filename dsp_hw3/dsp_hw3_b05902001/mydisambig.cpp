#include<iostream>
#include<stdio.h>
#include"Ngram.h"
#include<map>
#include<set>
#include<vector>
#include<string.h>
#include<stack>
using namespace std ;


int main(int argc, char* argv[]){
	Vocab voc;
	Ngram lm(voc,atoi(argv[8]));
	
	//const char lm_filename[] = "./bigram.lm";
	//File lmFile(lm_filename, "r" );
	File lmFile(argv[6], "r");
	lm.read(lmFile);
	lmFile.close();
		
	map< string, vector<string> > Zh2Ch;
	
	//File mapfile("ZhuYin-Big5.map", "r");
	File mapfile(argv[4],"r");	
	char* temp;
	while(temp = mapfile.getline()){
		
		//cout << temp << '\n';
		temp = strtok(temp,"\n");
		char *key = strtok(temp,"\t");
		//cout << key << "------" ;//string sent = (string)temp;
		char *value = strtok(NULL,"\t");
		//cout << value << '\n';
		string key_s = (string)key;
		//cout << key_s << "---------";		
		char *unit = strtok(value," ");
		//cout << unit;
		
		while(unit){
			//cout << unit ;
			string pick = (string)unit;
			Zh2Ch[key_s].push_back(pick);
			unit = strtok(NULL," ");
			//cout << unit;
		}	
	}

	//map.read(mapfile);
	//mapfile.close();
	
	//cout << "TERMINATE\n";

	
	//File test("testdata/seg_example.txt", "r");
	
	File test(argv[2], "r");
	char* readline;
	int spider = 1, count = 0;
	
	while(readline = test.getline()){
		/*
		if(count >= spider){
			break;
		}
		count++;
		*/
		//cout << line;
		//vector<string> store;
		//store.push_back("<s>");
		//VocabIndex wid;
		string line = (string)readline;
		//cout << line ;
		int pos = 0 ;
		while((pos = line.find(" ")) != -1){
     			line.erase(pos, 1);
    		}
		pos = line.find("\n");
		line.erase(pos,1);
		//cout << line ;
		//cout << "------------";

		vector<string> piece;
		for(int i=0 ; i<line.size(); i++){
			string word = line.substr(i,2);
			i++;
			piece.push_back(word);
			//cout << word;	
		}
		
		vector< vector<string> > route;
		for(int i=0 ; i < piece.size() ; i++){
			//cout << piece[i];
			vector<string> node ;
			for(int j=0 ; j<Zh2Ch[ piece[i] ].size(); j++ ){
				node.push_back(Zh2Ch[piece[i]].at(j));

				//cout << Zh2Ch[ piece[i]].at(j) ;
			}
			route.push_back(node);
		}
		
		//vector<string> predict;
		vector<int> index;
		VocabIndex empty_context[] = {Vocab_None};	
		double prob[2048][256] ;
		int come_from[2048][256] ;
			
		for(int i=0 ; i<route[0].size(); i++){
			string uni = route[0][i] ;
			VocabIndex wid = voc.getIndex(uni.c_str());
			if(wid == Vocab_None){
        			wid = voc.getIndex(Vocab_Unknown);
			}

			prob[i][0] =  lm.wordProb(wid, empty_context);
			come_from[i][0] = 0 ;
		}
		
		//cout << "end initail\n";

		for(int i=1 ; i<route.size() ; i++){		
			//double now_prob = 0 ;
			for(int j=0 ; j<route[i].size() ; j++){
				double max_prob = -10000 ;
				int max_index = 0 ;
				//string former = route[i][j-1];
				string latter = route[i][j];
				//VocabIndex wid1 = voc.getIndex(former.c_str());
				VocabIndex wid2 = voc.getIndex(latter.c_str());

				for(int k=0 ; k<route[i-1].size() ; k++){
					LogP now_prob ; 
					string former = route[i-1][k];
					VocabIndex wid1 = voc.getIndex(former.c_str());

					if(wid1 == Vocab_None){
        					wid1 = voc.getIndex(Vocab_Unknown);
					}
    					if(wid2 == Vocab_None){
        					wid2 = voc.getIndex(Vocab_Unknown);
					}
					VocabIndex context[] = {wid1, Vocab_None};
					now_prob = lm.wordProb(wid2,context);
					//cout << now_prob;
					//double uni_prob = lm.wordProb(wid2, empty_context);
					//now_prob += uni_prob;
					now_prob += prob[k][i-1];
					
					if(now_prob > max_prob){
						max_prob = now_prob;
						max_index = k;
						
					}
					//cout << route[i-1][k] << route[i][j] << '\n' ;
				}
				prob[j][i] = max_prob;
				come_from[j][i] = max_index;
				//cout << route[i-1][max_index];	
			}

		}

		//cout << "end Veterbi";

		stack<int> index_stack ;
		
		//for(int i=route[route.size()-2].size() ; i>=0; i--){
		int seek_index ;
		double pro = -10000;
		int len = route.size()-1;
		
		for(int j=0 ; j< route[len].size() ; j++){
			if(prob[j][len] > pro){
				pro = prob[j][len] ;
				seek_index = j ;
			}
		}

		for(int i=len ; i>=0; i--){
			index_stack.push(seek_index);
			seek_index = come_from[seek_index][i];
		}
		

		/*
		for(int i=0 ; i<route.size()-1 ; i++){
			//cout << ' ' <<index[i];
			//cout << ':' <<route[i].size();
			cout << ' ' <<route[i].at(index[i]) ;
			//cout << "------\n";
		}
		*/
		//cout << pro ;
		int step = 0;
		cout << "<s>" ;
		//cout << route[step][index_stack.top()];
		//index_stack.pop();
		//step++;
		
		while(!index_stack.empty()){
			//cout << index_stack.top();
			cout << ' ' << route[step][index_stack.top()];
			index_stack.pop();
			step++;
		}
		
		cout << " </s>" ;
			
		cout << '\n' ;
		

	}
	
	
}

