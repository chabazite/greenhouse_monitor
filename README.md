**Link to Webapp**: **<a href="http://greenhouseapp.s3-website.us-east-2.amazonaws.com" target="_blank">Greenhouse Monitoring Station</a>**
==============================================
Greenhouse Real-Time Data Stream & Predictions
=============================================

## Project Overview
<a name="Project Overview"></a>
The Greenhouse Monitoring Station is a personal project born out of a passion for both gardening and technology. As a hobbyist gardener and a professional Machine Learning (ML) engineer, this station represents the perfect intersection of these interests. While there are commercial smart controllers and sensor boards available, the goal of this project was to create a custom, DIY solution tailored specifically to my greenhouse.

This system will be equipped with a range of sensors to monitor critical environmental factors like air, water, and soil quality, ensuring the health and growth of plants. A key feature of this project is real-time data streaming, which not only provides immediate insights into the greenhouse environment but also feeds into a predictive model. This model, leveraging ML algorithms, forecasts the maximum values of various environmental parameters over the next hour.

Such predictive capabilities will allow for automated control of the greenhouse environment. Based on the forecasts, the system can intelligently operate relays to open or close windows, turn on fans, water plants, or refill water containers, among other actions. This level of automation not only makes greenhouse management more efficient but also serves as a practical application of ML skills in a real-world setting.

The project is more than a hobby; it's a personal exploration into how technology can enhance gardening practices. It serves as a testament to the power of integrating ML with everyday tasks and interests. As such, this Greenhouse Monitoring Station isn't just about keeping plants healthy – it's about the joy of building something functional and innovative, while honing skills that are crucial in my professional life as an ML engineer.

## Table of Contents
<details open>
  <summary>Show/Hide</summary>
  <br>
 
1. [ Project Organization ](#File_Description)
2. [ Technologies Used ](#Technologies_Used)    
3. [ Future Improvements ](#Future_Improvements)

</details>


## Project Organization

<details>
<a name="File_Description"></a>
<summary>Show/Hide</summary>
 <br>

    ├── LICENSE
    ├── .gitignore
    ├── README.md          <- The top-level README for developers using this project.
    ├──
    ├── .github
    │   └── workflows       <- Folder for CI/CD workflows using github Actions
    │       └── CI.yml       <- YAML file for github Actions
    │
    ├── images               <- Folder for images used in README
    │   └── README         
    │
    ├── requirements.txt   <- The requirements file for reproducing the analysis environment, e.g.
    │                         generated with `pip freeze > requirements.txt`
    |
    └── src                <- Source code for use in this project.
        │
        ├── hardware       <- Scripts to run the arduino board & sensor program
        |   ├── secrets.h  <- Location of information needed to connect to IOT and wifi
        │   └── main.cpp   <- main script for sensor board written in Arduino (C++ based)
        │
        └── webapp          <- Scripts to create a real-time websocket connection app
            ├── app.js      <- Scripts to connect to websocket API and recieve data
            └── index.html  <- Script to create the main structure of the web app in html
         
        
--------
  </details>   

## Technologies Used:
<details>
<a name="Technologies_Used"></a>
<summary>Show/Hide</summary>
<br>
    
![AWS Architecture](/images/WeatherStation.png)
      
    ├──AWS
        ├── IOT CORE
        ├── Kinesis Data Streams
        ├── Kinesis Firehose
        ├── Managed Service for Apache Flink
        ├── S3
        ├── Sagemaker
        |    ├── DeepAR
        |    └── Estimator     
        ├── Gateway API (Websocket)
        ├── Lambda
        ├── DynamoDB
        └── CloudWatch
    ├──Docker
    ├── Arduino
    |   ├── ESP_CLIENT_MAIL
    |   └── D11 AdaFruit
    ├──Python
    |   ├── Pandas
    |   ├── boto3
    |   └── s3fs
    ├── HTML
    └── JavaScript
        └── charts.js
 ------------
 </details>
  
## Future Improvements
 <a name="Future_Improvements"></a>
 <details>
<summary>Show/Hide</summary>
<br>
As the Greenhouse Monitoring Station project evolves, several enhancements are on the horizon to elevate its capabilities and efficiency. Here are the planned improvements and additional suggestions:

**Expansion of Sensor Array:** The current setup with temperature and humidity sensors is effective, but to gain a more comprehensive understanding of the greenhouse environment, additional sensors are in the pipeline. This includes water quality sensors to ensure optimal plant hydration, water level sensors to maintain adequate water supply, and soil moisture sensors to monitor and manage the watering needs of plants precisely. Additionally, considering the integration of soil quality sensors could provide valuable insights into nutrient levels and soil health, further aiding in the cultivation of healthier plants.

**Development of a Dedicated Controller Board:** A custom controller board is planned to be designed and implemented. This board will directly manage the operation of various relays, enabling more refined control over physical devices such as fans, watering systems, and window actuators. The aim is to create a more integrated and reliable system that responds accurately to the data insights and predictive models.

**Continuous Model Improvement:** As more data is collected, there will be a continuous focus on refining and enhancing the ML models. This involves not just fine-tuning the existing predictive algorithms but also exploring new modeling techniques that could offer more accurate or efficient predictions. This iterative process will help in adapting the system to changing environmental conditions and plant needs more effectively.

**Automated Alert System:** Implementing an automated alert system that notifies me of critical changes in the greenhouse environment or system malfunctions. This could be via email, SMS, or a mobile app notification, ensuring that I can respond promptly to any issues.

**Integration with Weather Forecast Data:** Incorporating external weather forecast data can provide a more holistic approach to predicting and adjusting the greenhouse environment. This could help in preempting changes in environmental conditions due to weather variations, leading to more proactive adjustments.

**Energy Consumption Monitoring:** Adding sensors to monitor the energy consumption of various devices in the greenhouse. This data can be used to optimize energy usage, making the system not only more cost-effective but also environmentally friendly.

**User Interface for Monitoring and Control:** Developing a user-friendly interface, possibly a web or mobile app, that allows me to monitor real-time data, control devices manually if needed, and view historical data trends. This could enhance the ease of managing and interacting with my greenhouse system.


</details>

<p><small>Project based on the <a target="_blank" href="https://drivendata.github.io/cookiecutter-data-science/">cookiecutter data science project template</a>. #cookiecutterdatascience</small></p>
<p>README outline tailored from [awesomeahi95][]<p>
