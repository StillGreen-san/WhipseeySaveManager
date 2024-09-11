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

/// Returns the function name, ignoring closures
///
/// # Example
///
/// ```rust
/// mod module {
/// # use whipseey_save_manager::fn_name;
///     pub fn function_name() {
///         assert_eq!(fn_name!(), "function_name");
///     }
/// }
/// # module::function_name();
/// ```
#[macro_export]
macro_rules! fn_name {
    () => {{
        fn type_name_of() -> &'static str {
            std::any::type_name_of_val(&type_name_of)
        }
        let full_name = type_name_of();
        let full_name =
            full_name[..full_name.len() - "::type_name_of".len()].trim_end_matches("::{{closure}}");
        &full_name[full_name.rfind(':').map(|i| i + 1).unwrap_or(0)..]
    }};
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn fn_name_function() {
        assert_eq!(fn_name!(), "fn_name_function");
    }

    #[allow(clippy::redundant_closure_call)]
    #[test]
    fn fn_name_closure() {
        (|| {
            (|| {
                assert_eq!(fn_name!(), "fn_name_closure");
            })();
        })();
    }

    #[test]
    fn fn_name_internal_name() {
        fn type_name_of() {
            assert_eq!(fn_name!(), "type_name_of");
        }
        type_name_of();
    }

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
