#[cfg(windows)]
extern crate winres;

fn main() {
    #[cfg(windows)]
    windows();
}

#[cfg(windows)]
fn windows() {
    const LID_ENGLISH_US: u16 = 0x0409;
    const VS_FF_PRERELEASE: u64 = 0x000002;
    const VS_FF_NONE: u64 = 0x000000;
    let file_flag = if std::env::var("CARGO_PKG_VERSION_PRE").unwrap().is_empty() {
        VS_FF_NONE
    } else {
        VS_FF_PRERELEASE
    };
    winres::WindowsResource::new()
        .set_icon("res/icon.ico")
        .set_language(LID_ENGLISH_US)
        .set_version_info(winres::VersionInfo::FILEFLAGS, file_flag)
        .compile()
        .expect("resource compiler failure");
}
