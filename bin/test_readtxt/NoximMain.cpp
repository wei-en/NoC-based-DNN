/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2010 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the implementation of the top-level of Noxim
 */

#include "NoximMain.h"
#include <vector>

using namespace std;

// need to be globally visible to allow "-volume" simulation stop
unsigned int drained_volume;

/***** Modified by CMH *****/ 
// Log file stream
// transient power
//ofstream results_log_pwr;
// transient temperature, should be moved to Thermal_IF
// ofstream results_log_temp;
//ofstream results_log_throughput;
//ofstream results_STLD;
//ofstream results_buffer;
//ofstream showstateslog;
//ifstream pir_table[64];
/*****      END       *****/ 

// Initialize global configuration parameters (can be overridden with command-line arguments)
int NoximGlobalParams::verbose_mode = DEFAULT_VERBOSE_MODE;
int NoximGlobalParams::trace_mode = DEFAULT_TRACE_MODE;
char NoximGlobalParams::trace_filename[128] = DEFAULT_TRACE_FILENAME;
int NoximGlobalParams::mesh_dim_x = DEFAULT_MESH_DIM_X;
int NoximGlobalParams::mesh_dim_y = DEFAULT_MESH_DIM_Y;
int NoximGlobalParams::mesh_dim_z = DEFAULT_MESH_DIM_Z;
int NoximGlobalParams::group_neu_num = DEFAULT_GROUP_NEU_NUM;	// 2018.09.01 edit by Yueh-Chi,Yang
int NoximGlobalParams::buffer_depth = DEFAULT_BUFFER_DEPTH;
int NoximGlobalParams::min_packet_size = DEFAULT_MIN_PACKET_SIZE;
int NoximGlobalParams::max_packet_size = DEFAULT_MAX_PACKET_SIZE;
int NoximGlobalParams::routing_algorithm = DEFAULT_ROUTING_ALGORITHM;
char NoximGlobalParams::routing_table_filename[128] = DEFAULT_ROUTING_TABLE_FILENAME;
int NoximGlobalParams::selection_strategy = DEFAULT_SELECTION_STRATEGY;
float NoximGlobalParams::packet_injection_rate = DEFAULT_PACKET_INJECTION_RATE;
float NoximGlobalParams::probability_of_retransmission = DEFAULT_PROBABILITY_OF_RETRANSMISSION;
int NoximGlobalParams::traffic_distribution = DEFAULT_TRAFFIC_DISTRIBUTION;
char NoximGlobalParams::traffic_table_filename[128] = DEFAULT_TRAFFIC_TABLE_FILENAME;
int NoximGlobalParams::simulation_time = DEFAULT_SIMULATION_TIME;
int NoximGlobalParams::stats_warm_up_time = DEFAULT_STATS_WARM_UP_TIME;
int NoximGlobalParams::rnd_generator_seed = 0;//time(NULL);
bool NoximGlobalParams::detailed = DEFAULT_DETAILED;
float NoximGlobalParams::dyad_threshold = DEFAULT_DYAD_THRESHOLD;
unsigned int NoximGlobalParams::max_volume_to_be_drained = DEFAULT_MAX_VOLUME_TO_BE_DRAINED;
vector <pair <int, double> > NoximGlobalParams::hotspots;
//***MODIFY BY HUI-SHUN***/
int NoximGlobalParams::throt_type = DEFAULT_THROTTLING_TYPE;
int NoximGlobalParams::down_level = DEFAULT_DOWN_LEVEL;
float	NoximGlobalParams::throt_ratio	= DEFAULT_THROTTLING_RATIO;
//---------------------------------------------------------------------------
//** 2018.09.01 edit by Yueh-Chi,Yang **//
char NoximGlobalParams::NNmodel_filename[128] = DEFAULT_NNMODEL_FILENAME;
char NoximGlobalParams::NNweight_filename[128] = DEFAULT_NNWEIGHT_FILENAME;
char NoximGlobalParams::mapping_filename[128] = DEFAULT_MAPPING_FILENAME;
//**************************************//
//** 2018.09.12 edit by Yueh-Chi, Yang **//
//deque < deque < deque <int> > >  NoximGlobalParams::throttling;
int NoximGlobalParams::throttling[128][128][1]; //** 2018.09.14 edit by Yueh-Chi,Yang **//
//***************************************//

int sc_main(int arg_num, char *arg_vet[])
{
	float temp_type;
	vector <float> temp;
	vector <float> temp2;
	vector <float> temp3;
	vector <float> temp4;
	ifstream fin_w("../vgg16_all.txt", ios::in);	
	while(fin_w >> temp_type){
		temp.push_back(temp_type);
	}

	cout<<"yes"<<temp.size()<<endl;
	fin_w.close();

	//temp.clear();
	ifstream fin_w1("../vgg16_all.txt", ios::in);	
	while(fin_w1 >> temp_type){
		temp2.push_back(temp_type);
	}

	cout<<"yes"<<temp2.size()<<endl;
	fin_w1.close();

	ifstream fin_w2("../vgg16_all.txt", ios::in);	
	while(fin_w2 >> temp_type){
		temp3.push_back(temp_type);
	}

	cout<<"yes"<<temp3.size()<<endl;
	fin_w2.close();

	ifstream fin_w3("../vgg16_all.txt", ios::in);	
	while(fin_w3 >> temp_type){
		temp4.push_back(temp_type);
	}

	cout<<"yes"<<temp4.size()<<endl;
	fin_w3.close();

    return 0;
}
