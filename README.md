# kalyna

### Compiling


```
git clone https://github.com/yhetman/kalyna
cd kalyna
make
```

To reocompile the code use:

```
make re
```

### Usage

```
./kalyna -k key -i ./file_path -o ./file_path [options]
```


Several `options` are available:
`-e` - to start encrypting process (is used by default);
`-d` - to start dencrypting process;
`-s` - name of standart to use;
`-h` - display help message.


### Available standarts

|       Name       |  Word size  |  Block size |  Key size | Number of Rounds  |
|       :---:      |    :----:   |     :---:   |    :--:   |       :---:       | 
| kalyna_128x128   |      64     |      128    |    128    |        10         |
| kalyna_128x256   |      64     |      128    |    256    |        14         |
| kalyna_256x256   |      64     |      256    |    256    |        14         |
| kalyna_256x512   |      64     |      256    |    512    |        18         |
| kalyna_512x512   |      64     |      512    |    512    |        18         |
