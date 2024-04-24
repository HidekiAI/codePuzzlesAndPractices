use std::{io::Read, result};

// From https://en.wikipedia.org/wiki/ROT13:
// ROT13 (Rotate13, "rotate by 13 places", sometimes hyphenated ROT-13) is a
// simple letter substitution cipher that replaces a letter with the 13th letter
// after it in the Latin alphabet. ROT13 is a special case of the Caesar cipher
// which was developed in ancient Rome.
// Because there are 26 letters (2×13) in the basic Latin alphabet, ROT13 is
// its own inverse; that is, to undo ROT13, the same algorithm is applied,
// so the same action can be used for encoding and decoding. The algorithm
// provides virtually no cryptographic security, and is often cited as a
// canonical example of weak encryption.
//      Input	ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
//      Output	NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm
//      Why did the chicken cross the road? -> Jul qvq gur puvpxra pebff gur ebnq?
//      To get to the other side! -> Gb trg gb gur bgure fvqr!
//
// Only rotate ASCII alphabetic characters, to ensure the result is still valid UTF-8.
struct RotDecoder<R: Read> {
    input: R,
    rot: u8,
}

// Implement the `Read` trait for `RotDecoder`.
impl<R: Read> Read for RotDecoder<R> {
    fn read(&mut self, dest_buffer_head: &mut [u8]) -> std::io::Result<usize> {
        // Do we assume that buffer is large enough to hold the entire input?
        let src_len = self.input.read(dest_buffer_head)?; // let it opt-out and return early using "?"

        // Rotate the buffer in-place.
        for char_index in 0..src_len {
            let ch = dest_buffer_head[char_index];
            if ch.is_ascii_alphabetic() {
                let base = if ch.is_ascii_lowercase() {
                    b'a' // binary literal 'a'
                } else {
                    b'A' // binary literal 'A'
                };
                dest_buffer_head[char_index] = base + (ch - base + self.rot) % 26;
            }
        }
        Ok(src_len)
    }
}

fn main() {
    let mut rot = RotDecoder {
        input: "Gb trg gb gur bgure fvqr!".as_bytes(),
        rot: 13,
    };
    let mut result = String::new();
    rot.read_to_string(&mut result).unwrap();
    println!("{}", result);
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn joke() {
        let mut rot = RotDecoder {
            input: "Gb trg gb gur bgure fvqr!".as_bytes(),
            rot: 13,
        };
        let mut result = String::new();
        rot.read_to_string(&mut result).unwrap();
        assert_eq!(&result, "To get to the other side!");
    }

    #[test]
    fn binary() {
        let input: Vec<u8> = (0..=255u8).collect();
        let mut rot = RotDecoder::<&[u8]> {
            input: input.as_ref(),
            rot: 13,
        };
        let mut buf = [0u8; 256];
        assert_eq!(rot.read(&mut buf).unwrap(), 256);
        for i in 0..=255 {
            if input[i] != buf[i] {
                assert!(input[i].is_ascii_alphabetic());
                assert!(buf[i].is_ascii_alphabetic());
            }
        }
    }
}
