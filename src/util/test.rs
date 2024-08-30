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
