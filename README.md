# ImageStegnography
The objective of this project is to send a secret text encoded inside an image of bmp file format.

Sample Execution
gcc *.c

./a.out -e beautiful.bmp secret.txt

*****************Selected Encoding*****************

Read and validate arguments is successful
Opened all the files successfully

.............Started Encoding.............

width = 1024
height = 768
Possible to hide the data in image
Successful :: copied header......
Successful :: Encoded the magic string 
Successful :: Encoded secret file extention size
Successful :: Encoded extension
Successful :: Encoded secret file size
Successful :: Encoded the secret data
Successful :: Copied remaining bytes
Hurrayy!!!!  Encoding is completed
./a.out -d stego.bmp decode.txt
*****************Selected Decoding*****************
Info :: Read and validate is successful
INFO :: Opened stego_image.bmp
Opened required files successfully
.............Started decoding.............
Successful :: Decoded Magic string signature
Successful :: Decoded secret file extension size
Successful :: Decoded secret file extension 
INFO :: Ouput file is not mentioned!! -- Creating decode.txt as default!!
Successful :: Opened output file 
Successful :: Decoded secret file size 
Successful ::  Decoded secret message
Successful :: Hurrayy!!! Decoding Completed :)
You can check text content by using the command --> cat decode.txt
