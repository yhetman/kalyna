# kalyna


### Benchmarks

|      Standart    |     1 MB    |    10 MB    |   100 MB    |       1 GB        |
|       :---:      |    :----:   |     :---:   |     :--:    |       :---:       | 
| kalyna_128x128   |   0m5.537s  |   0m56.486s |  11m8.006s  |         -         |
| kalyna_128x256   |   0m9.591s  |   1m42.884s |        -    |         -         |
| kalyna_256x256   |   0m10.609s |   1m53.801s |        -    |         -         |
| kalyna_256x512   |   0m15.280s |   2m24.180s |        -    |         -         |
| kalyna_512x512   |   0m18.974s |   2m47.893s |        -    |    195m59.878s    |


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
