# cpp_data_visualization
visualization of output data of PARCS (computer code for nuclear reactor) using ROOT (c++ framework https://root.cern.ch/)<br>
This analysis was performed in 2015, with ROOT version 5.2<br>


to run this code, you need to install ROOT (cern)<br>
 > .L showCoreConfig.C<br>
 > test()

for new ROOT version, you might need to modify the code to run it.

Algorithm used in this code: <br>
 - read data by condition<br>
 - clustering<br>

Input data in text format<br>
output of analysis:<br>
(see figures and video in folder output_Data/output_10percent and output_Data/output_100percent/)<br>
<img src="./docs/PowDistr_14_0_15.png" width="800">

video data at 10% power:<br>
[![Alt text](https://img.youtube.com/vi/0CeiG6oXPEk/0.jpg)](https://www.youtube.com/watch?v=0CeiG6oXPEk)

video data at 100% power:<br>
[![Alt text](https://img.youtube.com/vi/joDI_G37_9g/0.jpg)](https://www.youtube.com/watch?v=joDI_G37_9g)