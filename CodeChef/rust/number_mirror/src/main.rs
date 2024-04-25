

fn main() {
    // read in from stdin
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();

    // dump to stdout
    println!("{}", input);
}
