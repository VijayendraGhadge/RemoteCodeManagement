:run.bat file for OOD Project 3- Message Passing Communication

@echo off
echo.
echo "Peer.exe End Connection localhost 9080 localhost 9070 ../Hello.txt"
.\Debug\Peer.exe "End Receiver" localhost 9080 localhost 9070 ../Hello.txt

pause