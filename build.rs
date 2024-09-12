#[cfg(windows)]
extern crate winres;

fn main() {
    #[cfg(windows)]
    windows();
}

#[cfg(windows)]
fn windows() {
    const LID_ENGLISH_US: u16 = 0x0409;
    const VS_FF_PRERELEASE: u64 = 0x00000002;
    winres::WindowsResource::new()
        .set_icon("res/icon.ico")
        .set_language(LID_ENGLISH_US)
        .set_version_info(winres::VersionInfo::FILEFLAGS, VS_FF_PRERELEASE)
        .compile()
        .unwrap();
}
