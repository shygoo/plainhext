# plainhext
Plaintext commmented hex to binary converter.

## Usage
```
plainhext input_file.hex output_file.bin
```

## Format
* Mixed-case literal hex bytes with free whitespace
* Comments using `//` and `;`

## Example
```
6461 ; a comment
6E 6B // another comment
20 6D 656D6573
```

## Building
Run `make`.