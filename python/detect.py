import asyncio
import os

# pip install rich
from rich import inspect

# pip install winrt
from winrt.windows.media.faceanalysis import FaceDetector
from winrt.windows.graphics.imaging import BitmapDecoder, SoftwareBitmap
from winrt.windows.storage import StorageFile, FileAccessMode

async def main():

  # face detector class
  inspect(FaceDetector)

  # supported format
  detector = await FaceDetector.create_async()
  format = detector.get_supported_bitmap_pixel_formats()[0]
  inspect(format)
  
  # read image
  file = await StorageFile.get_file_from_path_async(os.fspath(os.getcwd() + r"\face.jpg"))
  stream = await file.open_async(FileAccessMode.READ)
  decoder = await BitmapDecoder.create_async(stream)
  bitmap = await decoder.get_software_bitmap_async()
  nv12 = SoftwareBitmap.convert(bitmap, format)
  inspect(nv12)

  # detect faces
  faces = await detector.detect_faces_async(nv12)
  inspect(faces[0].face_box)

asyncio.run(main())