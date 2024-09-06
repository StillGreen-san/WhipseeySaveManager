pub mod file;

#[cfg(test)]
pub mod test;

pub use file::load_ini_file;
pub use file::write_ini_file;
pub use file::write_ini_file_padded;
pub use file::LocateError;

pub fn for_each_window_mut<T, F>(slice: &mut [T], size: usize, mut function: F)
where
    F: FnMut(&mut [T]),
{
    for start in 0..=slice.len().saturating_sub(size) {
        function(&mut slice[start..][..size]);
    }
}
