Sample instruction
% ./noxim -dimx 8 -dimy 8 -dimz 1 -NNmodel my_model.txt -NNweight my_weight.txt -NNinput my_input.txt -mapping dir_x -routing xyx -groupsize 32 -sim 10000
% ./noxim -dimx 8 -dimy 8 -dimz 1 -NNmodel my_model.txt -NNweight my_weight.txt -NNinput my_input.txt -routing xyx -groupsize 1024 -sim 10000 -mapping table my_mapping/g1024_lyr_x.txt

For the DNN model grouping, I made my own mapping tables for Lyr_x mapping and Lyr_y mapping. 
The mapping table are in bin/my_mapping.


My DNN model: my_model.txt
The input: my_input.txt
The weights: my_weight.txt
_________________________________________________________________________
 Layer (type)                		Shape                    Param #   
=========================================================================
 input layer1                           784

 Hidden layer1 (Dense)                  256                      200960    
                                                                 
 Hidden layer2  (Dense)                 128                      32896     
                                                                 
 output layer (Dense)             	10                       1290       

=========================================================================
Total params: 235,146
