
## 📝 WinRT Face Detection Sample

![rich](https://img.shields.io/badge/rich%20%28pip3%29-12.6.0-blue.svg)
![winrt](https://img.shields.io/badge/winrt%20%28pip3%29-1.0.21033.1-green.svg)
![vs](https://img.shields.io/badge/visual%20studio-17.3.32929.385%20%282022%29-brightgreen.svg)
![platform](https://img.shields.io/badge/platform%20toolset-v143-pink.svg)
![flavor](https://img.shields.io/badge/flavor-win32/x64-yellow.svg)

Demonstrate the usage of [Windows.Media.FaceAnalysis.FaceDetector API](https://msdn.microsoft.com/library/windows/apps/windows.media.faceanalysis.facedetector.aspx). The repo provides basic samples for C++ (WRL) and python, in addition to the official [C#, cppwinrt](https://github.com/microsoft/Windows-universal-samples/tree/main/Samples/BasicFaceDetection), [C++/CX and VB](https://github.com/microsoft/Windows-universal-samples/tree/main/archived/BasicFaceDetection) guidance.


### 1️⃣ Windows Runtime C++ Template Library (WRL) Console App

Folder [cppwrl/](https://github.com/der3318/winrt-fd-sample/tree/main/cppwrl) is a Visual Studio 2022 solution to build a x86/x64 console app:

<img src="/cppwrl/demo.png">

It detects faces in picture [cppwrl/SampleInput.jpg](https://github.com/der3318/winrt-fd-sample/blob/main/cppwrl/SampleInput.jpg) and print the bounding box via stdout.


### 2️⃣ Python Script

Python script and its dependency are available under folder [python/](https://github.com/der3318/winrt-fd-sample/tree/main/python):

<img src="/python/demo.png">

Use `pip3 install -r requirements.txt` to install libs, and run `python3 detect.py` to [dump](https://github.com/Textualize/rich#rich-inspect) the detection info of image [python/face.jpg](https://github.com/der3318/winrt-fd-sample/blob/main/python/face.jpg).

