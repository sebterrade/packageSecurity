from discord_webhook import *
from datetime import datetime
import serial
import time
import string

Port = "COM6"
camlink = 'http://192.168.2.85'
# Signal = "boom"


arduino = serial.Serial(port=Port, baudrate=115200, timeout=.1)

def send_to_disc(myString, linked=0):
    webhook = DiscordWebhook(
        url='https://discord.com/api/webhooks/1045442153469460532/05czaqNQ_diHGOhr06gs_Ht4ZuebNP7len246aJTIJglhCNc8F5uKePEOsFLK77Jr4pC',
        content=myString,
        avatar_url="https://cdn.discordapp.com/avatars/1045442153469460532/ef39b36aff9a0f0487c4d662f0bcfb72.webp?size=80",
        username="viz"

    )
    if linked == 1:
        now = datetime.now()
        current_time_str = now.strftime("%D at %H:%M:%S")
        message = "**ALERT: Sensors detect your package has been moved.**\n\nCurrent Time: " + current_time_str + "\nClick the above link for a live camera feed."
        embed = DiscordEmbed(title='Camera Live Feed', description=message, color='FF0000',
                             url=camlink)
        embed.set_image("https://image.cnbcfm.com/api/v1/image/106331549-1578702858515packagetheftpkg.00_04_29_14.still003.jpg")
        webhook.add_embed(embed)
    response = webhook.execute()


flag = False


def send_if_keyword(data):
    word = data.decode('utf-8')
    word = word.strip()
    # word = data
    str1 = "Message : Authorized access" #A package has been left
    str2 = "Package safe" #A package has been correctly retrieved
    str3 = "Access denied" #A false card has been read
    str4 = "Message : Access denied"
    str5 = "THIEF" #A package has been stolen

    print(word)

    if word.__eq__(str1):
        send_to_disc("A package has been left at your door. Sensors are active.")
        return
    elif word.__eq__(str2):
        send_to_disc("Your package has been aquired safely.")
        return
    elif ( word.__eq__(str3) or word.__eq__(str4)):
        send_to_disc("Someone attempted to use an unregistered fob on your device.")
    elif word.__eq__(str5):
        send_to_disc("**ALERT: Your package has been stolen**", 1)
        return


while True:
    # This code is a placeholder that reads user input instead of the arduino
    data = arduino.readline()
    # data = input("What is the arduino saying?: ")
    send_if_keyword(data)
    time.sleep(0.05)
