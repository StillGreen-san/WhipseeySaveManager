use core::fmt;

pub mod ini {
    pub const DEFAULT: &str = include_str!("test/default.ini");
    pub const INVALID_KEYS: &str = include_str!("test/invalid-keys.ini");
    pub const INVALID_SECTION: &str = include_str!("test/invalid-sections.ini");
    pub const INVALID_VALUE_RANGES: &str = include_str!("test/invalid-value-ranges.ini");
    pub const INVALID_VALUE_TYPES: &str = include_str!("test/invalid-value-types.ini");
    pub const LENIENT_VALUES: &str = include_str!("test/lenient-values.ini");
    pub const VALID: &str = include_str!("test/valid.ini");
}

// copied from nightly https://doc.rust-lang.org/src/core/macros/mod.rs.html#154-179
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
// copied from nightly https://doc.rust-lang.org/src/core/panicking.rs.html#371-414
pub fn assert_matches_failed<T: fmt::Debug + ?Sized>(
    left: &T,
    right: &str,
    args: Option<fmt::Arguments<'_>>,
) -> ! {
    struct Pattern<'a>(&'a str);
    impl fmt::Debug for Pattern<'_> {
        fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
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
