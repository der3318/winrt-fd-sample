use windows::{
    Media::FaceAnalysis::FaceDetector,
    Graphics::Imaging::{BitmapDecoder, SoftwareBitmap},
    Storage::{StorageFile, FileAccessMode}
};

fn main() -> windows::core::Result<()> {
    // supported format
    let detector = FaceDetector::CreateAsync()?.get()?;
    for format in FaceDetector::GetSupportedBitmapPixelFormats()? {
        println!("{:?}", format);
    }

    // locate input image
    let mut path = std::env::current_dir().unwrap();
    path.push("face.jpg");
    println!("{:?}", path);

    // read image
    let file = StorageFile::GetFileFromPathAsync(&windows::core::HSTRING::from(path.into_os_string()))?.get()?;
    let stream = file.OpenAsync(FileAccessMode::Read)?.get()?;
    let decoder = BitmapDecoder::CreateAsync(&stream)?.get()?;
    let bitmap = decoder.GetSoftwareBitmapAsync()?.get()?;
    let nv12 = SoftwareBitmap::Convert(&bitmap, FaceDetector::GetSupportedBitmapPixelFormats()?.GetAt(0)?)?;

    // detect faces
    let faces = detector.DetectFacesAsync(&nv12)?.get()?;
    for face in faces {
        println!("{:?}", face.FaceBox()?);
    }

    // succeeded exit
    return Ok(());
}