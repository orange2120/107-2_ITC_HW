# 107-2 Introduction to Computer Homework

2019 Spring , Introduction to Computer, EE 1003 02

## Usage

### Build  
```
$ make
$ ./<Program name>
```

### Cleanup excutable files
```
$ make clean
```

## HW1 Bitmap image

### Programming Problem:  
  Rotate a picture conterclockwise by 90 degree.  

### Bonus: PrewiitEdge Detector  
  Implement PrewiitEdge Detector on bitmap picture.  

<table>
  <tr>
    <th>Origin</th>
    <th>PrewiitEdge detector</th>
  </tr>
  <tr>
    <td><img src="./HW1/img/flower.bmp" height="200px" alt="Origin"></td>
    <td><img src="./HW1/output/flower_ans.bmp" height="200px" alt="Applied PrewiitEdge detector"></td>
  </tr>
</table>

### Run test  
```
$ make test
$ ./test <Image path>
```
`<Image path>`: images need to be stored in `./img` folder. Ex: `./test test.bmp`

## HW2 Machine Instruction Simulator

### Programming Problem:  
Machine Instruction Simulator

### Bonus: AssemblySimulator