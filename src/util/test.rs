//! Test utilities.

/// ini strings for testing
pub mod ini {
    pub const DEFAULT: &str = include_str!("test/default.ini");
    pub const INVALID_KEYS: &str = include_str!("test/invalid-keys.ini");
    pub const INVALID_SECTIONS: &str = include_str!("test/invalid-sections.ini");
    pub const INVALID_VALUE_RANGES: &str = include_str!("test/invalid-value-ranges.ini");
    pub const INVALID_VALUE_TYPES: &str = include_str!("test/invalid-value-types.ini");
    pub const LENIENT_VALUES: &str = include_str!("test/lenient-values.ini");
    pub const VALID: &str = include_str!("test/valid.ini");
}

/// Asserts that an expression matches the provided pattern.
///
/// This macro is generally preferable to `assert!(matches!(value, pattern))`, because it can print
/// the debug representation of the actual value shape that did not meet expectations. In contrast,
/// using [`assert!`] will only print that expectations were not met, but not why.
///
/// The pattern syntax is exactly the same as found in a match arm and the `matches!` macro. The
/// optional if guard can be used to add additional checks that must be true for the matched value,
/// otherwise this macro will panic.
///
/// On panic, this macro will print the value of the expression with its debug representation.
///
/// Like [`assert!`], this macro has a second form, where a custom panic message can be provided.
///
/// copied from nightly https://doc.rust-lang.org/src/core/macros/mod.rs.html#154-179
///
/// # Examples
///
/// ```
/// # use crate::whipseey_save_manager::assert_matches;
///
/// let a = Some(345);
/// let b = Some(56);
/// assert_matches!(a, Some(_));
/// assert_matches!(b, Some(_));
///
/// assert_matches!(a, Some(345));
/// assert_matches!(a, Some(345) | None);
///
/// // assert_matches!(a, None); // panics
/// // assert_matches!(b, Some(345)); // panics
/// // assert_matches!(b, Some(345) | None); // panics
///
/// assert_matches!(a, Some(x) if x > 100);
/// // assert_matches!(a, Some(x) if x < 100); // panics
/// ```
#[macro_export]
macro_rules! assert_matches {
    ($left:expr, $(|)? $( $pattern:pat_param )|+ $( if $guard: expr )? $(,)?) => {
        match $left {
            $( $pattern )|+ $( if $guard )? => {}
            ref left_val => {
                $crate::util::test::assert_matches_failed(
                    left_val,
                    std::stringify!($($pattern)|+ $(if $guard)?),
                    std::option::Option::None
                );
            }
        }
    };
    ($left:expr, $(|)? $( $pattern:pat_param )|+ $( if $guard: expr )?, $($arg:tt)+) => {
        match $left {
            $( $pattern )|+ $( if $guard )? => {}
            ref left_val => {
                $crate::util::test::assert_matches_failed(
                    left_val,
                    std::stringify!($($pattern)|+ $(if $guard)?),
                    std::option::Option::Some(std::format_args!($($arg)+))
                );
            }
        }
    };
}
/// internal assert_matches function
///
/// copied from nightly https://doc.rust-lang.org/src/core/panicking.rs.html#371-414
pub fn assert_matches_failed<T: std::fmt::Debug + ?Sized>(
    left: &T,
    right: &str,
    args: Option<std::fmt::Arguments<'_>>,
) -> ! {
    struct Pattern<'a>(&'a str);
    impl std::fmt::Debug for Pattern<'_> {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            f.write_str(self.0)
        }
    }
    let right = &Pattern(right);

    match args {
        Some(args) => panic!(
            r#"assertion `left matches right` failed: {args}
  left: {left:?}
 right: {right:?}"#
        ),
        None => panic!(
            r#"assertion `left matches right` failed
  left: {left:?}
 right: {right:?}"#
        ),
    };
}
