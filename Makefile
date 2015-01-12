objects =  Serial_Base_API.o main.o 
main : $(objects)
	cc -o main $(objects) 
	rm *.o
forwin : $(objects)
	cc -o main $(objects)  
	rm *.o
clean:
	rm main
	rm main.exe
