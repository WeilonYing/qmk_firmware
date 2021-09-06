#!/usr/bin/python3

# Credit https://stackoverflow.com/a/19082750
import cv2
import sys

def to_bw(greyscale_val):
    return 0 if greyscale_val < 128 else 1

def writePrefix(fileHandle, count, width, height):
    fileHandle.write(
f"""
// animation file
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define ANIM_FRAME_COUNT  {count}
#define ANIM_FRAME_WIDTH  {width}
#define ANIM_FRAME_HEIGHT {height}

typedef struct PACKED {{
    bool frames[ANIM_FRAME_COUNT][ANIM_FRAME_HEIGHT][ANIM_FRAME_WIDTH];
}} anim_frames_t;

"""
    )

def writeSuffix(fileHandle):
    fileHandle.write('#endif\n')

def writeEndComma(fileHandle, index, lastIndex):
    if index < lastIndex:
        fileHandle.write(',')
    fileHandle.write('\n')

def writeFrames(fileHandle, frames):
    # write variable declaration
    fileHandle.write('const anim_frames_t g_frames = {{\n')
    # write frames
    for i, frame in enumerate(frames):
        fileHandle.write('  {\n')
        for j, row in enumerate(frame):
            fileHandle.write('    { ')
            for k, column in enumerate(row):
                fileHandle.write(f'{column}')
                if k < len(row) - 1:
                    fileHandle.write(', ')
            fileHandle.write(' }')
            writeEndComma(fileHandle, j, len(frame) - 1)
        fileHandle.write('  }')
        writeEndComma(fileHandle, i, len(frames) - 1)

    # write end of variable declaration
    fileHandle.write('}};\n')

if len(sys.argv) < 2:
    print('Usage: python3 processvideo.py <filename>')
    sys.exit(0)

FILEPATH = sys.argv[1]
cap = cv2.VideoCapture(FILEPATH)
while not cap.isOpened():
    cap = cv2.VideoCapture(FILEPATH)
    cv2.waitKey(1000)
    print("Wait for the header")
frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
# frame_count = 300 # debug
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
with open("./output.txt", 'w') as f:
    frame_output = []
    writePrefix(f, frame_count, frame_width, frame_height)
    while True:
        flag, frame = cap.read()
        if flag:
            pos_frame = int(cap.get(cv2.CAP_PROP_POS_FRAMES))
            # The frame is ready and already captured'
            grey_frame = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
            cv2.imshow('video', frame)
            
            for x in range(len(grey_frame)):
                for y in range(len(grey_frame[x])):
                    if grey_frame[x][y] < 64:
                        grey_frame[x][y] = 0
                    else:
                        grey_frame[x][y] = 1
            print (str(pos_frame)+" frames of", cap.get(cv2.CAP_PROP_FRAME_COUNT))
            frame_output.append(grey_frame)
            # print()
            # print(grey_frame)
            # cv2.waitKey(16)

        if cv2.waitKey(10) == 27:
            break
        if not flag or cap.get(cv2.CAP_PROP_POS_FRAMES) >= cap.get(cv2.CAP_PROP_FRAME_COUNT):
            # If the number of captured frames is equal to the total number of frames,
            # we stop
            break
    writeFrames(f, frame_output)
    writeSuffix(f)
