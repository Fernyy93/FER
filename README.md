# FER
Reanimating this project after neglecting self learning for 7 years. 
Facial expression recognition using OpenCV 4.9.0 in C++
Data acquired from the Cohn-Kanade database which can be found here: http://www.consortium.ri.cmu.edu/ckagree/ (link no longer works, need to find alternative). 
Uses the LBP (circular) algorithm for feature extraction and will use a multi-class SVM to classify images with facial expressions.

This started as a final year thesis topic of my undergraduate degree in Electrical Engineering. 
The original topic was to make an android phone application to recognize facial expressions however my experience in programming was
severely limited so I implemented it in MATLAB instead. 

For the purpose of learning I decided to rewrite it in C++ but paused for 7 years due to personal reasons.
7 years later I intend to finish it and get the software in a well written state. 
This software currently incorrectly recognizes each expression as neutral due to the overwhelming amount of neutral expressions in my dataset.
After I'm happy with the infrastructure of the project I will either make the dataset more balanced or change the feature extraction or classification.

Acknowledgements:  
Kanade, T., Cohn, J. F., & Tian, Y. (2000). Comprehensive database for facial expression analysis. Proceedings of the Fourth IEEE International Conference on Automatic Face and Gesture Recognition (FG'00), Grenoble, France, 46-53.  
and  
Lucey, P., Cohn, J. F., Kanade, T., Saragih, J., Ambadar, Z., & Matthews, I. (2010). The Extended Cohn-Kanade Dataset (CK+): A complete expression dataset for action unit and emotion-specified expression. Proceedings of the Third International Workshop on CVPR for Human Communicative Behavior Analysis (CVPR4HB 2010), San Francisco, USA, 94-101.
