/*
 * NN-Noxim - the NoC-based ANN Simulator
 *
 * (C) 2018 by National Sun Yat-sen University in Taiwan
 *
 * This file contains the implementation of loading NN model
 */

#include "NNModel.h"
#include <iomanip>
#include <math.h>
#include <ctime>
#include <string>

NNModel::NNModel()
{
}

bool NNModel::load()//M_fname Useless tytyty
{
	cout<<"model file loading (filename: " << NoximGlobalParams::NNmodel_filename << ")..."<< endl;		//** 2018.09.02 edit by Yueh-Chi,Yang **//
	ifstream fin(NoximGlobalParams::NNmodel_filename, ios::in); 						//** 2018.09.02 edit by Yueh-Chi,Yang **//
	char temp_type[20], temp_sv_pad[20], temp_actfun[10];
	int temp;
	int temp_c_x, temp_c_y, temp_z, temp_num, temp_std, temp_x, temp_y, temp_pad;
	int input_size, output_size;
	deque< deque< int > > conv;
	deque< deque< int > > pool;
	int all_Nue=0;
	// *****************all layer Neu_num setting*******************
	cout<<endl;
	cout<<"layer_ID |    type | Neu_num |       X |       Y | channel |   C/P_X |   C/P_Y |  stride | padding | act_fun |"<<endl;
	cout<<"--------------------------------------------------------------------------------------------------------------"<<endl;

	while(fin >> temp_type){


		if (!strcmp( temp_type, "Dense"))
		{
			all_leyer_type.push_back('f');
			char line[256];
			fin.getline(line, sizeof(line) - 1);
			sscanf(line, "%d %s", &temp, &temp_actfun);
			deque< int > temp_leyer_size;
			temp_leyer_size.push_back(temp);
			if(!strcmp( temp_actfun, "relu"))
				temp_leyer_size.push_back(RELU);
			else if(!strcmp( temp_actfun, "tanh"))
				temp_leyer_size.push_back(TANH);
			else if(!strcmp( temp_actfun, "sigmoid"))
				temp_leyer_size.push_back(SIGMOID);
			else if(!strcmp( temp_actfun, "softmax"))
				temp_leyer_size.push_back(SOFTMAX);

			all_leyer_size.push_back(temp_leyer_size);
			all_Nue+=temp;
			cout<<setw(8)<<all_leyer_type.size()-1<<" |"<<    setw(8)<<"Fully"<<" |"<<setw(8)<<temp_leyer_size[0]<<" |"
				<<                   setw(10)<<" |"<<            setw(10)<<" |"<<                   setw(10)<<" |"
				<<                   setw(10)<<" |"<<            setw(10)<<" |"<<                   setw(10)<<" |"
				<<                   setw(10)<<" |"<<setw(8)<<temp_actfun<<" |"<<endl;
		}
		else if (!strcmp( temp_type, "Input"))
		{
			all_leyer_type.push_back('i');
			char line[256];
			fin.getline(line, sizeof(line) - 1);
			sscanf(line, "%d %d %d", &temp_x, &temp_y, &temp_z);
			temp = temp_x * temp_y*temp_z;
			deque< int > temp_leyer_size;
			temp_leyer_size.push_back(temp);
			temp_leyer_size.push_back(temp_x);
			temp_leyer_size.push_back(temp_y);
			temp_leyer_size.push_back(temp_z);

			all_leyer_size.push_back(temp_leyer_size);
			all_Nue+=temp;
			cout<<setw(8)<<all_leyer_type.size()-1<<" |"<<           setw(8)<<"Input"<<" |"<<setw(8)<<temp_leyer_size[0]<<" |"
				<<setw(8)<<temp_leyer_size[1]<<" |"<<setw(8)<<temp_leyer_size[2]<<" |"<<setw(8)<<temp_leyer_size[3]<<" |"
				<<                   setw(10)<<" |"<<                   setw(10)<<" |"<<                   setw(10)<<" |"
				<<                   setw(10)<<" |"<<                   setw(10)<<" |"<<endl;
		}
		else if (!strcmp( temp_type, "%"))
		{
		
			char line[256];
			fin.getline(line, sizeof(line) - 1);
		}
		else
		{
			cout<<"!!Error model format: "<<temp_type<<" !!"<<endl;
			char line[256];
			fin.getline(line, sizeof(line) - 1);
		}

	}
	cout<<"model all_leyer complete"<<endl;
	cout<<"all neu:"<<all_Nue<<endl;
	fin.close();
	input_size=all_leyer_size.front()[0];
	output_size=all_leyer_size.back()[0];

	//******************mapping information prepare************************
	mapping_table.clear();
	mapping_table.assign( NoximGlobalParams::mesh_dim_x*NoximGlobalParams::mesh_dim_y, -1 );

	if(!strcmp(NoximGlobalParams::mapping_algorithm, "random") )
	{
		srand( time(NULL) );
		for( int i = 0; i < mapping_table.size() ; i++ )
		{
			while(1)
			{
				int map_point = rand() % mapping_table.size();
				if( mapping_table[map_point]==-1 )
				{
					mapping_table[map_point] = i;
					break;
				}

			}
		}
	}
	else if(!strcmp(NoximGlobalParams::mapping_algorithm, "dir_x") )
		for( int i = 0; i < mapping_table.size() ; i++ )	//dir_x mapping
			mapping_table[i]=i;
	else if(!strcmp(NoximGlobalParams::mapping_algorithm, "dir_y") )	
		for( int i = 0; i < mapping_table.size() ; i++ )	//dir_y mapping
			mapping_table[i]= (i%NoximGlobalParams::mesh_dim_x)*NoximGlobalParams::mesh_dim_x + (i/NoximGlobalParams::mesh_dim_x);	
	else if(!strcmp(NoximGlobalParams::mapping_algorithm, "table") )	
	{
		ifstream fin_m(NoximGlobalParams::mapping_table_filename, ios::in);						//** 2018.09.02 edit by Yueh-Chi,Yang **
		cout<<"mapping file loading (filename: " << NoximGlobalParams::mapping_table_filename << ")..."<< endl;		//** 2018.09.02 edit by Yueh-Chi,Yang **
		while(!fin_m.eof()){
			char line[256];
			fin_m.getline(line, sizeof(line) - 1);
			if (line[0] != '\0') {
			    	if (line[0] != '%') {
					int ID_Group, ID_PE;
					sscanf(line, "%d %d", &ID_Group, &ID_PE);
					mapping_table[ID_Group] = ID_PE;
			    	}
			}
		}
	}

	else
	{
		cout<<"Error mapping algorithm!!"<<endl;
		exit(1);
	}

	cout<<"maping complete"<<endl;

	// ******************temp_Group_table setting**********************
	int temp_ID_Neu=0;
	int temp_layer=0;
	int temp_ID_In_layer=0;
	int temp_ID_Group=0;
	int temp_ID_In_Group=0;
	float temp_w;
	int temp_ID_conv = -1;
	deque < float > temp_conv_weight;
	int temp_layer_maxID = input_size;

	Group_table.clear();
	deque < NeuInformation > temp_Group_table;
	temp_Group_table.clear();
	deque < int > temp_leyer_ID_Group;
	temp_leyer_ID_Group.clear();
	
	ifstream fin_w(NoximGlobalParams::NNweight_filename, ios::in);	
	cout<<"weight file loading (filename: " << NoximGlobalParams::NNweight_filename << ")..."<<endl;	//** 2018.09.02 edit by Yueh-Chi,Yang **//

	while(1)
	{
		NeuInformation NeuInfo;

		NeuInfo.ID_Neu = temp_ID_Neu;

		if(temp_ID_Neu < temp_layer_maxID){									
			NeuInfo.ID_layer = temp_layer;
			NeuInfo.ID_In_layer = temp_ID_In_layer;

			if( temp_ID_In_Group >=  NoximGlobalParams::group_neu_num){					//** 2018.09.01 edit by Yueh-Chi,Yang **// change group
				Group_table.push_back(temp_Group_table);
        			temp_Group_table.clear();

				temp_ID_In_Group = 0;
				temp_leyer_ID_Group.push_back(temp_ID_Group);
				temp_ID_Group++;
			}
			NeuInfo.ID_In_Group = temp_ID_In_Group;
			NeuInfo.ID_Group = temp_ID_Group;
		}
		else{													//change layer and group
			Group_table.push_back(temp_Group_table);
			temp_Group_table.clear();
					
			NeuInfo.ID_layer = ++temp_layer;
			temp_leyer_ID_Group.push_back(temp_ID_Group);
			all_leyer_ID_Group.push_back(temp_leyer_ID_Group);
			temp_leyer_ID_Group.clear();
			NeuInfo.ID_Group = ++temp_ID_Group;

			if(temp_ID_Neu >= all_Nue) break;

			if(temp_ID_Group >= NoximGlobalParams::mesh_dim_x*NoximGlobalParams::mesh_dim_y)
				cout<<"error group_size or NoC_size" <<endl;						//output error!! 
			
			temp_ID_In_layer = 0;
			temp_ID_In_Group = 0;
			NeuInfo.ID_In_layer = temp_ID_In_layer;
			NeuInfo.ID_In_Group = temp_ID_In_Group;
			temp_layer_maxID += all_leyer_size[temp_layer][0];
		}

		NeuInfo.Type_layer = all_leyer_type[NeuInfo.ID_layer];
		
		if( NeuInfo.Type_layer == 'f')
		{
			for( int i=0 ; i < ( all_leyer_size[ temp_layer-1 ][0] +1 ) ; i++ )	//include bias***
			{
				fin_w>>temp_w;
				NeuInfo.weight.push_back(temp_w);
			}
		}

		//***********mapping****************
		
		temp_Group_table.push_back(NeuInfo);
		temp_ID_In_Group++;
		temp_ID_In_layer++;
		temp_ID_Neu++;
	}

	fin_w.close();
	
	deque<float>().swap(temp_conv_weight);
	deque<NeuInformation>().swap(temp_Group_table);
	deque<int>().swap(temp_leyer_ID_Group);
	cout<<"model & group complete"<<endl;

//******************print floorplan****************

	cout<<"Hardware floorplan:"<<endl;
	cout<<"  ";	
	for(int i=0;i<NoximGlobalParams::mesh_dim_x;i++)
	cout<<"-----";	
	cout<<"-"<<endl;	
	for(int j=0;j<NoximGlobalParams::mesh_dim_y;j++)
	{
		cout<<"  |";
		for(int i=0;i<NoximGlobalParams::mesh_dim_x;i++)
		{

			int loacl_id = j*NoximGlobalParams::mesh_dim_x + i;
			int x;
				for(x=0;x<mapping_table.size();x++)
				{
					if(mapping_table[x]==loacl_id) break;
				}
				if(x<Group_table.size())
				{
					int temp_lay=Group_table[x][0].ID_layer;
					cout<<setw(3)<<temp_lay<<" |";
				}
				else
					cout<<setw(3)<<" "<<" |";

		}
		cout<<endl<<"  ";
		for(int i=0;i<NoximGlobalParams::mesh_dim_x;i++)
		cout<<"-----";	
		cout<<"-"<<endl;
	}
	//***********input setting***************

	fstream fin_in(NoximGlobalParams::NNinput_filename, ios::in); 

	float temp_in;
	int i = -1;
	deque< float > temp_data_in;
	temp_data_in.clear();

	while(fin_in >> temp_in){
    		i++;
		temp_data_in.push_back(temp_in);
		if(i==input_size-1)
		{
			all_data_in.push_back(temp_data_in);
    			temp_data_in.clear();
			i=-1;
		}
	}

	deque<float>().swap(temp_data_in);
	fin_in.close();

	cout<<"all_data_in.size(): "<<all_data_in.size()<<endl;
	cout<<"load input complete"<<endl;



	return true;
}



