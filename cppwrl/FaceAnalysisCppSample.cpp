#include <chrono>
#include <codecvt>
#include <iostream>
#include <locale>
#include <regex>
#include <thread>

#pragma comment(lib, "runtimeobject.lib")
#include <Windows.Foundation.h>
#include <Windows.Graphics.Imaging.h>
#include <Windows.Media.FaceAnalysis.h>
#include <Windows.Storage.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Graphics::Imaging;
using namespace ABI::Windows::Media::FaceAnalysis;
using namespace ABI::Windows::Storage;
using namespace ABI::Windows::Storage::Streams;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

template <typename T, typename U>
HRESULT WaitAndGetResults(ComPtr<IAsyncOperation<T*>> asyncOp, U ptrRef)
{
    constexpr long MaxTrials = 1000;
    constexpr long RoundInMs = 10;
    HRESULT hr = E_PENDING;
    for (long t = 0; (t < MaxTrials) && FAILED(hr); t++)
    {
        hr = asyncOp->GetResults(ptrRef);
        std::this_thread::sleep_for(std::chrono::milliseconds(RoundInMs));
    }
    return hr;
}

int main(int /*argc*/, char* argv[])
{
    // runtime init
    RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);

    // locate input image
    std::string path = std::regex_replace(argv[0], std::regex("FaceAnalysisCppSample.exe"), "SampleInput.jpg");
    std::wstring imgPath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(path);

    // static class methods
    ComPtr<IFaceDetectorStatics> faceDetectorStatics;
    ComPtr<IStorageFileStatics> storageFileStatics;
    ComPtr<IBitmapDecoderStatics> bitmapDecoderStatics;
    ComPtr<ISoftwareBitmapStatics> softwareBitmapStatics;
    (void)GetActivationFactory(HStringReference(RuntimeClass_Windows_Media_FaceAnalysis_FaceDetector).Get(), faceDetectorStatics.ReleaseAndGetAddressOf());
    (void)GetActivationFactory(HStringReference(RuntimeClass_Windows_Storage_StorageFile).Get(), storageFileStatics.ReleaseAndGetAddressOf());
    (void)GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Imaging_BitmapDecoder).Get(), bitmapDecoderStatics.ReleaseAndGetAddressOf());
    (void)GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Imaging_SoftwareBitmap).Get(), softwareBitmapStatics.ReleaseAndGetAddressOf());

    // test face detector interface
    boolean isSupported = false;
    (void)faceDetectorStatics->get_IsSupported(&isSupported);
    std::cout << std::boolalpha << static_cast<bool>(isSupported) << std::noboolalpha << std::endl;

    // create detector instance
    ComPtr<IAsyncOperation<FaceDetector*>> faceDetectorAsync;
    ComPtr<IFaceDetector> faceDetector;
    (void)faceDetectorStatics->CreateAsync(faceDetectorAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(faceDetectorAsync, faceDetector.ReleaseAndGetAddressOf());

    // test max and min size getter
    BitmapSize maxSize, minSize;
    (void)faceDetector->get_MaxDetectableFaceSize(&maxSize);
    (void)faceDetector->get_MinDetectableFaceSize(&minSize);
    std::cout << maxSize.Height << " " << minSize.Width << std::endl;

    // read image
    ComPtr<IAsyncOperation<StorageFile*>> storageFileAsync;
    ComPtr<IStorageFile> storageFile;
    (void)storageFileStatics->GetFileFromPathAsync(HStringReference(imgPath.c_str()).Get(), storageFileAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(storageFileAsync, storageFile.ReleaseAndGetAddressOf());

    // get aceess to the stream
    ComPtr<IAsyncOperation<IRandomAccessStream*>> streamAsync;
    ComPtr<IRandomAccessStream> stream;
    (void)storageFile->OpenAsync(FileAccessMode_Read, streamAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(streamAsync, stream.ReleaseAndGetAddressOf());

    // create image decoder instance of the stream
    ComPtr<IAsyncOperation<BitmapDecoder*>> decoderAsync;
    ComPtr<IBitmapDecoder> decoder;
    (void)bitmapDecoderStatics->CreateAsync(stream.Get(), decoderAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(decoderAsync, decoder.ReleaseAndGetAddressOf());

    // cast into frame
    ComPtr<IBitmapFrameWithSoftwareBitmap> bitmapFrame;
    (void)decoder.As(&bitmapFrame);

    // read bitmap
    ComPtr<IAsyncOperation<SoftwareBitmap*>> bitmapAsync;
    ComPtr<ISoftwareBitmap> bitmap;
    (void)bitmapFrame->GetSoftwareBitmapAsync(bitmapAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(bitmapAsync, bitmap.ReleaseAndGetAddressOf());

    // convert bitmap into nv12 format
    ComPtr<ISoftwareBitmap> nv12;
    (void)softwareBitmapStatics->Convert(bitmap.Get(), BitmapPixelFormat_Nv12, nv12.ReleaseAndGetAddressOf());

    // do detection
    ComPtr<IAsyncOperation<IVector<DetectedFace*>*>> facesAsync;
    ComPtr<IVector<DetectedFace*>> faces;
    (void)faceDetector->DetectFacesAsync(nv12.Get(), facesAsync.ReleaseAndGetAddressOf());
    (void)WaitAndGetResults(facesAsync, faces.ReleaseAndGetAddressOf());

    // get first detected face
    ComPtr<IDetectedFace> face;
    (void)faces->GetAt(0, face.ReleaseAndGetAddressOf());

    // dump box info
    BitmapBounds box;
    (void)face->get_FaceBox(&box);
    std::cout << box.X << " " << box.Y << " " << box.Width << " " << box.Height << std::endl;

    return 0;
}