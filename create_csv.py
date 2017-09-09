import sys
import os.path

emotions = ("anger",
            "contempt",
            "disgust",
            "fear",
            "happy",
            "neutral",
            "sadness",
            "surprise")

BASE_PATH = "/home/ferny/Dropbox/QUT/Semester-1-2016/BEB801/Data/sorted_set/"
SEPARATOR = ";"


def main():
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
