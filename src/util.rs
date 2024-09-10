//! Various utilities and helpers.

pub mod file;

#[cfg(test)]
pub mod test;

pub use file::find_bfs_settings_path;
pub use file::find_savegame_path;
pub use file::load_ini_file;
pub use file::trim_to_existing_path;
pub use file::write_ini_file;
pub use file::write_ini_file_padded_no_spacing;
pub use file::LocateError;

/// Executes `function` for all contiguous windows of length `size` in `slice`. The windows overlap.
/// If the slice is shorter than `size`, the function is not called.
///
/// free function alternative to the [std::slice::Windows] iterator,
/// taken from `kpreid` https://internals.rust-lang.org/t/a-windows-mut-method-on-slice/16941/11
///
/// # Example
///
/// ```
/// # use whipseey_save_manager::util::for_each_window_mut;
/// let mut slice = [1, 2, 3, 2, 3, 1];
/// for_each_window_mut(&mut slice, 3, |win| {
///     if win == [1, 2, 3] {
///         win[2] = 1;
///     }
/// });
/// assert_eq!(slice, [1, 2, 1, 2, 1, 1]);
/// ```
///
pub fn for_each_window_mut<T, F>(slice: &mut [T], size: usize, mut function: F)
where
    F: FnMut(&mut [T]),
{
    for start in 0..=slice.len().saturating_sub(size) {
        function(&mut slice[start..][..size]);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn for_each_window_mut_duplicate_new_lines() {
        let mut data = Vec::from(b"line\n\nline\nline\n\nline\n");
        for_each_window_mut(&mut data, 2, |window| {
            if window == b"\n\n" {
                window[1] = 0;
            }
        });
        assert_eq!(data, b"line\n\0line\nline\n\0line\n");
    }
}
