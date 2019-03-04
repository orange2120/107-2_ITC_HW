# 107-2 Introduction to Computer Homework

2019 Spring , Introduction to Computer, EE 1003 02

## Usage

Build  
```
make
./<Program name>
```

Cleanup
```
make clean
```

## HW1 Bitmap image

### Programming Problem:  
  Rotate a picture conterclockwise by 90 degree.  

### Bonus:  PrewiitEdge Detector  
  Implement PrewiitEdge Detector on bitmap picture.  

<figure class="center">
    <img src="https://raw.githubusercontent.com/orange2120/107-2_ITC_HW/master/HW1/img/flower.bmp" height="300px" alt="Origin">
    <img src="https://raw.githubusercontent.com/orange2120/107-2_ITC_HW/master/HW1/output/flower_ans.bmp" height="300px" alt="Applied PrewiitEdge detector">
</figure>

Usage
```
make
./bmp
```

Run test  
```
make test
./test <Image path>
```
<Image path>: images need to be stored in `./img` folder. Ex: `./test test.bmp`