/*********
   Instructions for running the file
   Run with one command line arguments 
   1. Policy name (processor_sharing, water_filling, max_weight)
   2. K value
   3. R value
   4. N value

   Example: g++ romil_infinite.cpp -o romil_infinite
            ./romil_infinite romil_infinite 20 5

    Output: A csv file with the name kvalue_rvalue_Nvalue_filename.csv will be generated 
            in the results/infinite folder
   
*********/

#include<bits/stdc++.h>
#include <random>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
using namespace std;
using std::ofstream;
#define MAX 1000

int get_queue_length(int ik, int** queue);

int main(int argc, char* argv[])
{
    // int K = 20;
    int K = atoi(argv[2]);
    int R = atoi(argv[3]);
    int N = atoi(argv[4]);                             //server capacity
    double p[K];
    double served_till_now[K];              //packets served at that link
    double avg_link_delay[K-R];             //the value link stores to control rate for TCP links only
    double avg_server_delay[K];             //the value of avg delay as stored by server at each link
    
    // open a file to store the results
    string filename = argv[1];
    string kvalue = argv[2];
    string rvalue = argv[3];
    string Nvalue = argv[4];
    string filepathtowrite = "./results/infinite/"+kvalue+"_"+rvalue+"_"+Nvalue+"_"+filename+".csv";
    ofstream myfile;
    myfile.open(filepathtowrite);
    myfile << "p" << '\t'; 
    myfile << "Server_Utilisation" << '\t';
    myfile << "UDP_throughput" << '\t';
    myfile << "TCP_throughput" << '\t';
    myfile << "UDP_delay" << '\t';
    myfile << "TCP_delay" << '\n';

    for(int px=2;px<10;px++){               //to iterate over p the arrival rate from 0.2 to 0.9
    for(int i=0;i<K;i++){                   //setting to initial values
        p[i] = px*0.1;
        served_till_now[i] = 0;
        avg_server_delay[i] = 0;
        if(i<K-R)avg_link_delay[i] = 0;
    }
    
    int** incoming_queues = new int*[K];
 
    for(int i=0;i<K;i++){
        incoming_queues[i] = new int[MAX];
    }
    

    for(int i=0;i<K;i++){                   //value of -1 at queue means that there is no packet there
        for(int j=0;j<MAX;j++){             //any other value denotes the time at which that packet arrived at the link
            incoming_queues[i][j] = -1;
        }
    }
    
    int tmax = 10000;                       //to simulate over time
    int total_server_cap = 0;               
    //double server_util = 0;
    
    for(int t=0;t<tmax;t++){
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        srand(seed);
        for(int i=0;i<K;i++){
            double pr = (double)rand()/RAND_MAX;            //random generator to get arrivals with rate p
            if(pr<=p[i]){
                int P;
                P = t;
                int ql = get_queue_length(i,incoming_queues);
                incoming_queues[i][ql] = P;
            }    
        }
        int total_packets = 0;                              //to keep check of total packets in all queues
        for(int i=0;i<K;i++){
            total_packets = total_packets + get_queue_length(i,incoming_queues);
        }
        
        double probab = (double) K/N;                       //binomial distribution for server capacity

        unsigned seed2 = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed2); 
        binomial_distribution<int> distribution(N,probab); 

        int n = distribution(generator);
        total_server_cap += n;                      //total server capacity to find server utility
        if(n>=total_packets){                       //if server capacity more than total packets then reset server capacity to total packets.
            n = total_packets;
        }
        int served = 0;
        
        if (strcmp(argv[1], "processor_sharing") == 0) {
            // serve the packets in round robin fashion
            int l = n/K;
            for(int i=0;i<K;i++){
                for(int ii=0;ii<l;ii++){
                    int len = get_queue_length(i,incoming_queues);
                    if(len>0){
                        int P;
                        P = incoming_queues[i][0];
                        served_till_now[i] = served_till_now[i]+1;
                        for(int j=0;j<len;j++){    
                            incoming_queues[i][j] = incoming_queues[i][j+1];
                        }
                        avg_server_delay[i] = (double) ((served_till_now[i]-1)*avg_server_delay[i] + (t-P))/served_till_now[i];
                        if(i>R){
                            //for the K-R special links, update avg link delay using given formula
                            avg_link_delay[i] = (0.9*avg_link_delay[i] + 0.1*(t-P));    
                        }
                        served++;
                    }    
                }
            }
                    
            n = n - served;

            // serve the remaining packets randomly (Processor Sharing)
            int counter = 0;
            while(counter<n){
                int ii = 0;
                int max_size = 0;  
                // serve a random link from 0 to K-1
                do {
                    ii = rand()%K;
                } while(get_queue_length(ii, incoming_queues)==0);
                // ii = rand()%K;
                // cout << "ii " << ii << endl;
                max_size = get_queue_length(ii,incoming_queues);
                served_till_now[ii] = served_till_now[ii]+1;
                served++;
                int P;
                P = incoming_queues[ii][0];
                for(int j=0;j<max_size;j++){
                    incoming_queues[ii][j] = incoming_queues[ii][j+1];
                }
                avg_server_delay[ii] = (double) ((served_till_now[ii]-1)*avg_server_delay[ii] + (t-P))/served_till_now[ii];
                if(ii>R){
                    //for the K-R special links, update avg link delay using given formula
                    avg_link_delay[ii] = (0.9*avg_link_delay[ii] + 0.1*(t-P));    
                }
                        
            counter++;
            }
        }

        else if(strcmp(argv[1], "water_filling") == 0){
            // serve the packets in round robin fashion
            int l = n/K;
            for(int i=0;i<K;i++){
                for(int ii=0;ii<l;ii++){
                    int len = get_queue_length(i,incoming_queues);
                    if(len>0){
                        int P;
                        P = incoming_queues[i][0];
                        served_till_now[i] = served_till_now[i]+1;
                        for(int j=0;j<len;j++){    
                            incoming_queues[i][j] = incoming_queues[i][j+1];
                        }
                        avg_server_delay[i] = (double) ((served_till_now[i]-1)*avg_server_delay[i] + (t-P))/served_till_now[i];
                        if(i>R){
                            //for the K-R special links, update avg link delay using given formula
                            avg_link_delay[i] = (0.9*avg_link_delay[i] + 0.1*(t-P));    
                        }
                        served++;
                    }    
                }
            }
                    
            n = n - served;

            // serve the remaining packets randomly (Processor Sharing)
            int counter = 0;
            while(counter<n){
                int ii = 0;
                int max_size = 0;  
                // serve a random link from 0 to K-1
                do {
                    ii = rand()%K;
                } while(get_queue_length(ii, incoming_queues)==0);
                // ii = rand()%K;
                // cout << "ii " << ii << endl;
                max_size = get_queue_length(ii,incoming_queues);
                served_till_now[ii] = served_till_now[ii]+1;
                served++;
                int P;
                P = incoming_queues[ii][0];
                for(int j=0;j<max_size;j++){
                    incoming_queues[ii][j] = incoming_queues[ii][j+1];
                }
                avg_server_delay[ii] = (double) ((served_till_now[ii]-1)*avg_server_delay[ii] + (t-P))/served_till_now[ii];
                if(ii>R){
                    //for the K-R special links, update avg link delay using given formula
                    avg_link_delay[ii] = (0.9*avg_link_delay[ii] + 0.1*(t-P));    
                }
                        
            counter++;
            }
        }

        else {
            // if max-weight then serve all the packets to the link with maximum queue length
            while(n>0){
                int ii = 0;
                int max_size = 0;
                // find the link with maximum queue length
                for(int i=0;i<K;i++){
                    if(get_queue_length(i,incoming_queues)>max_size){
                        max_size = get_queue_length(i,incoming_queues);
                        ii = i;    
                    }
                    else if(get_queue_length(i,incoming_queues)==max_size){
                        if(avg_server_delay[i]>avg_server_delay[ii]){
                            ii = i;
                        }
                    }
                }    
                max_size = get_queue_length(ii,incoming_queues);
                // cout << "max_size: " << max_size << endl;
                if (max_size <= n) {
                    // update the delay by iterating max_size times
                    for (int m=0; m<max_size; m++){
                        served_till_now[ii] = served_till_now[ii]+1;
                        served++;
                        int P;
                        P = incoming_queues[ii][0];
                        // cout << "P in max<n: " << P << endl;
                        for (int j=0; j<max_size; j++) {
                            incoming_queues[ii][j] = incoming_queues[ii][j+1];
                        }
                        avg_server_delay[ii] = (double) ((served_till_now[ii]-1)*avg_server_delay[ii] + (t-P))/served_till_now[ii];
                        if(ii>R){
                            //for the K-R special links, update avg link delay using given formula
                            avg_link_delay[ii] = (0.9*avg_link_delay[ii] + 0.1*(t-P));
                        }
                    }
                    served = served + max_size;
                    n = n - max_size;
                }

                else {
                    // update the delay by iterating n times
                    for (int m=0; m<n; m++) {
                        for (int j=0; j<=n; j++) {
                            incoming_queues[ii][j] = incoming_queues[ii][j+1];
                        }
                        served_till_now[ii] = served_till_now[ii]+1;
                        int P;
                        P = incoming_queues[ii][0];
                        avg_server_delay[ii] = (double) ((served_till_now[ii]-1)*avg_server_delay[ii] + (t-P))/served_till_now[ii];
                        if(ii>R){
                            //for the K-R special links, update avg link delay using given formula
                            avg_link_delay[ii] = (0.9*avg_link_delay[ii] + 0.1*(t-P));
                        }

                    }
                    served = served + n;
                    n = 0;
                }
            }
        }

        //Update rates
        double delta = 0.1;
        for(int i=0;i<K-R;i++){
            int P;
            P = incoming_queues[i+R][0];
            int ti = P;
            
            //If some packet unserved and came before 1.2 avg link delay, decrease rate to 2/3rd
            if((t-ti)>ceil(1.2*avg_link_delay[i])){
                if(ti!=-1){
                    p[i+R] = 2*p[i+R]/3;
                }
            }
            
            //If all packets that came inside 1.2 avg link delay from current time served, increase rate
            else if((t-ti)<=ceil(1.2*avg_link_delay[i]) || ti == -1){
                double new_p = p[i+R] + delta;
                if(new_p>=0.9)new_p=0.9;
                p[i+R] = new_p;
            }
        }    
    }
    
    int served_total = 0;
    
    double throughput = 0;
    double UDP_tp = 0;
    double TCP_tp = 0;
    
    //Find the throughput for UDP links
    for(int i=0;i<R;i++){
        throughput = (double) served_till_now[i]/tmax;
        UDP_tp += throughput;
        served_total += served_till_now[i];
    }
    UDP_tp = UDP_tp/R;
    throughput = 0;
    
    //Find the throughput for TCP links
    for(int i=R;i<K;i++){
        throughput = (double) served_till_now[i]/tmax;
        served_total += served_till_now[i];
        TCP_tp += throughput;
    }
    TCP_tp = TCP_tp/(K-R);
    
    
    //Find server utility
    double server_util = (double) served_total / total_server_cap ;
    
    double UDP_del = 0;
    double TCP_del = 0;
    
    //Find the avg delay over UDP links
    for(int i=0;i<R;i++){
        UDP_del += avg_server_delay[i];
    }
    UDP_del = UDP_del/R;
    
    //Find the avg delay over TCP links
    for(int i=R;i<K;i++){
        TCP_del += avg_server_delay[i];
    }
    TCP_del = TCP_del/(K-R);
    
    //can check crucial outputs here
    cout<<"server utilisation: "<<server_util<<endl;
    cout<<"UDP throughput: "<<UDP_tp<<endl;
    cout<<"TCP throughput: "<<TCP_tp<<endl;
    cout<<"UDP delay: "<<UDP_del<<endl;
    cout<<"TCP delay: "<<TCP_del<<endl;
    
    cout<<endl;
    // write all the results in a csv file
    myfile << px * 0.1 << '\t';
    myfile << server_util << '\t';
    myfile << UDP_tp << '\t';
    myfile << TCP_tp << '\t';
    myfile << UDP_del << '\t';
    myfile << TCP_del << '\n';

    }
    myfile.close();
    return 0;
}


int get_queue_length(int ik,int** queue){
    for(int j=0;j<MAX;j++){
        if(queue[ik][j]==-1){
            return j;
        }    
    }
    return MAX;
}