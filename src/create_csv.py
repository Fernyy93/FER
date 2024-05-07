import sys
import os.path


def main():
    emotions = ["anger", "contempt", "disgust", "fear", "happy", "neutral", "sadness", "surprise"]
    

    BASE_PATH = "C:/Users/ferni/OneDrive/Computer Science/FER/FER2.0/new/"
    SEPARATOR = ";"
    
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for subdirname in dirnames:
            subject_path = os.path.join(dirname, subdirname)
            filename = subdirname+'.csv'
            f = open(filename, 'w')
            for filename in os.listdir(subject_path):
                abs_path = "%s/%s" % (subject_path, filename)
                fn = "%s%s" % (abs_path, SEPARATOR)
                f.write(fn+'\n')
    
    for emotion in emotions:
        filename = emotion+'.csv'
        f = open(filename,'w')          
        label = 0        
        for num in os.listdir(BASE_PATH+emotion):
            if os.path.isfile(os.path.join(BASE_PATH, emotion, num)):
                f.write(str(BASE_PATH+emotion+"/"+num+SEPARATOR+str(label))+',\n')
                label+=1
            else:
                print("Can't find folder")
        f.close()
     
    
if __name__ == "__main__":
    main()
