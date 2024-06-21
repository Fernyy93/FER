"""
This file is responsible for harvesting CK database for images of emotions. It gets a neutral face and a emotion face for each subject.
Based on Paul van Gent's code from blog post: http://www.paulvangent.com/2016/04/01/emotion-recognition-with-python-opencv-and-a-face-dataset/
Updated to python 3 and bug fixed
"""
import glob
import os
import pathlib
from shutil import copyfile

import cv2

# data set folders are structured as follows:
# Participant
# -- Session
# ---- emotion expression
# Each image in emotion expression folder is a frame of a video filming the participant making the expression. The last frame is the full expression to train the model on.
# THE FIRST FRAME OF EACH VIDEO IS THE NEUTRAL EXPRESSION, THAT IS WHY THERE ARE SOOOOO MANY
# Each expression corresponds to a number from 0 - 7
# 0 : neutral
# 1 : anger
# 2 : contempt
# 3 : disgust
# 4 : fear
# 5 : happy
# 6 : sadness
# 7 : surprise

def remove_old_set(emotions):
    """
    Removes old images produced from dataset.
    :param emotions: List of emotions names.
    """
    print("Removing old dataset")
    for emotion in emotions:
        filelist = glob.glob("sorted_set/%s/*" % emotion)
        for f in filelist:
            os.remove(f)

## why is images empty? failure to open file should have been caught by try catch block
def harvest_dataset(emotions):
    """
    Copies photos of participants in sessions to new folder.
    :param emotions: List of emotions names.
    for each participant
        for each session
            copy last image to the folder the expression belongs to
            if neutral = false
                get the first image and put in neutral folder
                neutral = true
            end if
        end for
    end for
    """
    print("Harvesting dataset")
    participants = glob.glob('source_emotion/*')  # returns a list of all folders with participant numbers
    #print("participants: " + str(participants))

    for participant in participants:
        neutral_added = False
        ## why is the exception being raised even though the code within the try block works?
        for sessions in glob.glob("%s/*" % participant):  # store list of sessions for current participant
            for files in glob.glob("%s/*" % sessions):
                current_session = files[20:-30]
                # exception is being raised even though code in the try block works
                file = open(files, 'r')
                        

                # emotions are encoded as a float, readline as float, then convert to integer
                emotion = int(float(file.readline()))
                
                image_path = base_path + "/source_images/%s/%s/*" % (participant[-4:], current_session)
                images = glob.glob(image_path)
                
                if not images:
                    print("images is empty")
                    break

                # get path for last image in sequence, which contains the emotion               
                source_filename = images[-1].split('/')[-1]
                # do same for emotion containing image
                destination_filename = "sorted_set/%s/%s" % (emotions[emotion], source_filename)
                # copy file
                copyfile("source_images/%s/%s/%s" % (participant[-4:], current_session, source_filename), destination_filename)

                if not neutral_added:
                    # do same for neutral image
                    source_filename = images[0].split('/')[-1]
                    # generate path to put neutral image
                    destination_filename = "sorted_set/neutral/%s" % source_filename
                    # copy file                    
                    copyfile("source_images/%s/%s/%s" % (participant[-4:], current_session, source_filename), destination_filename)
                    neutral_added = True

def find_faces(gray):
    # TODO finish this function with code from face_detect.py
    # add the code to save the image to standard size.
    face = faceDet.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
    face2 = faceDet2.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
    face3 = faceDet3.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
    face4 = faceDet4.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
    return [face, face2, face3, face4]            


def extract_faces(emotions):
    """
    Crops faces in emotions images.
    :param emotions: List of emotions names.
    """
    print("Extracting faces")
    for emotion in emotions:
        photos = glob.glob('sorted_set/%s/*' % emotion)

        for file_number, photo in enumerate(photos):
            frame = cv2.imread(photo)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) #Convert image to grayscale
            if gray is None:
                print("image is empty!")
                continue
            else:
                
                normalized_faces = find_faces(gray)

                os.remove(photo) # Delete the old photo

                # Go over detected faces, stop at first detected face, return empty if not a face
                if len(normalized_faces[0]) == 1:
                    facefeatures = normalized_faces[0]
                elif len(normalized_faces[1]) == 1:
                    facefeatures == normalized_faces[1]
                elif len(normalized_faces[2]) == 1:
                    facefeatures = normalized_faces[2]
                elif len(normalized_faces[3]) == 1:
                    facefeatures = normalized_faces[3]
                else:
                    facefeatures = ""
                    #print "noface"
                for (x,y,w,h) in facefeatures: # get co-ordinates and size of rectangle containing face
                    gray = gray[y:y+h, x:x+w] # cut frame to size

                    try:
                        out = cv2.resize(gray, (400, 400)) # resize face so all images have the same size
                        cv2.imwrite("sorted_set/%s/%s.png" % (emotion, file_number + 1), out) # write image after resizing
                    except:
                        print("error in processing %s " % photo)
                        continue

if __name__ == '__main__':
    emotions = ['neutral', 'anger', 'contempt', 'disgust', 'fear', 'happy', 'sadness', 'surprise']
    base_path = str(pathlib.Path().resolve())
    face_path = '/home/alex/opencv/opencv-4.x/data/haarcascades/'
    fd1 = 'haarcascade_frontalface_default.xml'
    fd2 = 'haarcascade_frontalface_alt2.xml'
    fd3 = 'haarcascade_frontalface_alt.xml'
    fd4 = 'haarcascade_frontalface_alt_tree.xml'

    faceDet = cv2.CascadeClassifier(face_path + fd1)
    print(faceDet.empty())
    faceDet2 = cv2.CascadeClassifier(face_path + fd2)
    print(faceDet.empty())
    faceDet3 = cv2.CascadeClassifier(face_path + fd3)
    print(faceDet.empty())
    faceDet4 = cv2.CascadeClassifier(face_path + fd4)
    print(faceDet.empty())
    remove_old_set(emotions)
    harvest_dataset(emotions)
    extract_faces(emotions)
