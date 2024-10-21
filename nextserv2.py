import requests
from datetime import datetime
import serial
import json  # Import json to format the dictionary into a string
ltos=[]
art=[]
# Initialize the serial connection
def clear_articles():
    global art
    art=[]
ser = serial.Serial('COM3', 115200)  # Replace 'COM4' with the correct port
def get_news(api_key, query):
    url = 'https://newsapi.org/v2/everything'
    params = {
        'apiKey': api_key,
        'q': query,  # Search query for Chennai news
        'pageSize': 6,  # Number of articles to return
        'sortBy': 'recent'  # Sort by relevancy to the query
    }

    response = requests.get(url, params=params)
    if response.status_code == 200:
        articles = response.json().get('articles', [])
        return articles
    else:
        print(f"Error: {response.status_code}, {response.text}")
        return []
def create_articles(articles):
    for i, article in enumerate(articles, start=1):
        #print(f"{i}. {article['title']}")
        #print(f"   Source: {article['source']['name']}")
        #print(f"   URL: {article['url']}")
        #print(article['title'])
        art.append("@@"+article['title']+"@@")
def get_weather(api_key, lat, lon):
    url = 'https://api.openweathermap.org/data/2.5/weather'
    params = {
        'lat': lat,   # Latitude
        'lon': lon,   # Longitude
        'appid': api_key,  # Your API key from OpenWeatherMap
        'units': 'metric',  # Units: 'metric' for Celsius
        'name': 'chennai'
    }

    response = requests.get(url, params=params)
    if response.status_code == 200:
        z = response.json()
        
        # Convert the JSON dictionary to a string and then encode it into bytes
        weather_string = json.dumps(z)  # Convert dictionary to a string
        #ser.write(weather_string.encode('utf-8'))  # Send as bytes
        
        print(z)  # Print weather data to the console
        return z
    else:
        print(f"Error: {response.status_code}, {response.text}")
        return None

def display_weather(data):
    global ltos
    if data:
       # loc=f"{data['name']}, {data['sys']['country']}"
        loc="Semmancheri"
        temp=f"{data['main']['temp']}degC"
        feel=f"{data['main']['feels_like']}degC"
        min_temp=f"{data['main']['temp_min']}degC"
        max_temp=f"{data['main']['temp_max']}degC"
        hum=f"Hum:{data['main']['humidity']}%"
        pres=f"{data['main']['pressure']}hPa"
        weat=f"{data['weather'][0]['main'].capitalize()}"
        weath_desc=f"{data['weather'][0]['description'].capitalize()}"
        cld_cvr=f"{data['clouds']['all']}%"
        vis=f"{data['visibility']}meters"
        win_spee=f"{data['wind']['speed']} m/s"
        win_dir=f"{data['wind']['deg']}deg"
        ltos=[loc,temp,feel,min_temp,max_temp,hum,pres,weat,weath_desc,cld_cvr,vis,win_spee,win_dir]
        
        print(f"Location: {data['name']}, {data['sys']['country']}")
        print(f"Temperature: {data['main']['temp']}°C")
        print(f"Feels Like: {data['main']['feels_like']}°C")
        print(f"Min Temperature: {data['main']['temp_min']}deg C")
        print(f"Max Temperature: {data['main']['temp_max']}deg C")
        print(f"Humidity: {data['main']['humidity']}%")
        print(f"Pressure: {data['main']['pressure']} hPa")
        print(f"Weather: {data['weather'][0]['main'].capitalize()}")
        print(f"Weather: {data['weather'][0]['description'].capitalize()}")
        print(f"Cloud Cover: {data['clouds']['all']}%")
        print(f"Visibility: {data['visibility']} meters")
        print(f"Wind Speed: {data['wind']['speed']} m/s, Wind Direction: {data['wind']['deg']}deg")

        # Sunrise and Sunset Time Conversion
        sunrise_time = datetime.utcfromtimestamp(data['sys']['sunrise']).strftime('%Y-%m-%d %H:%M:%S')
        sunset_time = datetime.utcfromtimestamp(data['sys']['sunset']).strftime('%Y-%m-%d %H:%M:%S')
        print(f"Sunrise: {sunrise_time} UTC")
        print(f"Sunset: {sunset_time} UTC")
    else:
        print("No weather data available.")

if __name__ == '__main__':
    #api_key = 'bc0bfb1129eb9801bb474fd3e3d3aa33'  # Replace with your OpenWeatherMap API key
    #lat = 13.112927285201197  # Latitude for Chennai
    #lon = 80.22439757094801   # Longitude for Chennai
    #weather_data = get_weather(api_key, lat, lon)
   # display_weather(weather_data)
#    print(ltos)
    #du=str(ltos)
   # du=du[1::]
    #du=du[:-1:]
    #print(du)
    #ser.write(du.encode('utf-8'))
    #print("sent data")

    try:
        api_key = '98b563e22024494580a7ea2f562b73fb'  # Replace with your actual API key
        query = 'chennai'  # Set the query to 'Chennai' for Chennai news
        articles = get_news(api_key, query)
        create_articles(articles)
        arb1=str(art)[1::]
        news_str_to_send=arb1[:-1:]
        print(news_str_to_send)
        #news_str_to_send=news_str_to_send.replace('"',"'")
        ser.write(news_str_to_send.encode('utf-8'))
        art=[]        
        while True:
            if ser.in_waiting > 0:
                # Handle potential decoding errors by ignoring them
                data = ser.readline().decode('utf-8', errors='ignore').rstrip()
                print(f"Received from ESP32: {data}")
                if "REQUESTING WEATHER DATA" in data:
                    api_key = 'bc0bfb1129eb9801bb474fd3e3d3aa33'  # Replace with your OpenWeatherMap API key
                    #api_key = 'e2830a2511ccc9e6571ba4895394bdcc'
                    lat = 12.810326 # Latitude for Chennai
                    lon =  80.245691    # Longitude for Chennai
                    weather_data = get_weather(api_key, lat, lon)
                    display_weather(weather_data)
                #    print(ltos)
                    du=str(ltos)
                    du=du[1::]
                    du=du[:-1:]
                    print(du)
                    ser.write(du.encode('utf-8'))
                    print("sent data")
                elif "REQUESTING NEWS DATA" in data:
                    api_key = '98b563e22024494580a7ea2f562b73fb'  # Replace with your actual API key
                    query = 'chennai'  # Set the query to 'Chennai' for Chennai news
                    articles = get_news(api_key, query)
                    create_articles(articles)
                    arb1=str(art)[1::]
                    news_str_to_send=arb1[:-1:]
                    print(news_str_to_send)
                    #news_str_to_send=news_str_to_send.replace('"',"'")
                    ser.write(news_str_to_send.encode('utf-8'))
                    art=[]
                    
                                    
            
    except KeyboardInterrupt:
        print("Exiting Program")

    finally:
        ser.close()
