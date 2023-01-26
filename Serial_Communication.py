"""By Henry"""
import serial as lib_serial

def invert_name(name):
    """ This function inverts the string
    received and adds sequential numbers between 
    letters """
    counter = 0
    new_name = ''
    for index in range(len(name) - 1, -1, -1):
        new_name += name[index]
        new_name += str(counter)
        counter += 1
    return new_name

port_name = input("Which port do you wanna connect to? ")
baud_rate = int(input("Baud rate: "))
port = lib_serial.Serial(port_name, baud_rate)

while True:
    task = input("Do you want to read or write? (if you wanna write your name type -- write_my_name) ")
    if task == "write":
        message = input("Message to write: ")
        port.write(bytes(message, 'UTF-8'))
    elif task == "read":
        ##number_of_bytes = int(input("How many bytes do you wanna read? "))
        ##reading = port.read(number_of_bytes)
        reading = port.readline()
        print(reading.decode()) #Default value UTF-8
    elif task == "write_my_name":
        name = input("What is your name? ")
        message = invert_name(name)
        port.write(bytes(message, 'UTF-8'))

    if 0xff == ord('q'):
        break






