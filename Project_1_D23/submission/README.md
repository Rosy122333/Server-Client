                            HOW TO COMPILE
One should upload and see the submitted folders in the linux.wpi.edu server. Once one sees both the client_folder and the server_folder they will be ready to compile. 
    To compile the client:
        - type "make" into the terminal inside the "client_folder" folder
        - type "make clean" to get rid of the .o file and type "make" to get the .o file again
        - type "./client [-options] server_url port_number"
            - Example "./client www.google.com 80"
            - "./client www.example.com 80"
             - "./client -p www.google.com 80"
            - "./client -p www.example.com 80"

    To compile the server:
        - type "make" to make the .o file for the server. You must be inside "server_folder" to run the command.
         - type "make clean" to get rid of the .o server file and type "make" to get it again.
        - type "./server port_number" to run
            - Example "./server 8000"
       
    To have the client connect to the socket the server made.
        - Both "client" and "server" will have at to be made using the "make" command beforehand. you must have the .o of the server running with a port number. Then you will run the client with that given socket number.
            - Example run "./server 5555"
            - then run "./client linux.wpi.edu/TMDG.html 5555"
