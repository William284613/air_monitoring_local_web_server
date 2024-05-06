const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" rel="stylesheet">
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 2.0rem; }
      p { font-size: 2.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 15px;
      }
    </style>
  </head>
  <body>
    <h2>Assignment 1 SKIH3113 Sensor Based System (A232)</h2>
    <h1>NodeMCU ESP8266 Air Monitoring System</h1>

    <p>
      <i class="fa fa-thermometer-half" style="font-size:3.0rem;color:#62a1d3;"></i> 
      <span class="dht-labels">Temperature : </span> 
      <span id="TemperatureValue">0</span>
      <sup class="units">&deg;C</sup>
    </p>
    <p>
      <i class="fa fa-tint" style="font-size:3.0rem;color:#75e095;"></i>
      <span class="dht-labels">Humidity : </span>
      <span id="HumidityValue">0</span>
      <sup class="units">%</sup>
    </p>
    
    <p>
       <span class="dht-labels">Gas Concentration : </span> 
       <span id="GasValue">0</span>
      <sup class="units">ppm</sup>
    </p>

    <p>
       <span class="dht-labels">Air Quality : </span> 
       <span id="AirQuality">0</span>
    </p>

    <p>
      <i class="far fa-clock" style="font-size:1.0rem;color:#e3a8c7;"></i>
      <span style="font-size:1.0rem;">Time </span>
      <span id="time" style="font-size:1.0rem;"></span>
      
      <i class="far fa-calendar-alt" style="font-size:1.0rem;color:#f7dc68";></i>
      <span style="font-size:1.0rem;">Date </span>
      <span id="date" style="font-size:1.0rem;"></span>
    </P>
  
    <script>

      // Set intervals to call functions repeatedly
      setInterval(function() {
        // Call a function repetatively with 2 Second interval
        getTemperatureData();
        getHumidityData();
        getGasData();
      }, 2500); 
      
      setInterval(function() {
        // Call a function repetatively with 1 Second interval
        Time_Date();
      }, 1000); 

      // Function to fetch temperature data from server
      function getTemperatureData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // Update temperature value on the web page
            document.getElementById("TemperatureValue").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readTemperature", true);
        xhttp.send();
      }

      // Function to fetch humidity data from server
      function getHumidityData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // Update humidity value on the web page
            document.getElementById("HumidityValue").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readHumidity", true);
        xhttp.send();
      }

      // Function to fetch gas data from server
      function getGasData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            // Parse response text to get gas value and air quality
              var gasData = this.responseText.split(',');
              var gasValue = gasData[0];
              var airQuality = gasData[1];
            // Update gas value and air quality on the web page
            document.getElementById("GasValue").innerHTML = gasValue;
            document.getElementById("AirQuality").innerHTML = airQuality; 
            }
        };
        xhttp.open("GET", "readGasSensor", true);
        xhttp.send();
      }

      
      // Function to update time and date
      function Time_Date() {
        var t = new Date();
        // Update time on the web page
        document.getElementById("time").innerHTML = t.toLocaleTimeString();
        var d = new Date();
        const dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday","Saturday"];
        const monthNames = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"];
        // Update date on the web page
        document.getElementById("date").innerHTML = dayNames[d.getDay()] + ", " + d.getDate() + "-" + monthNames[d.getMonth()] + "-" + d.getFullYear();
      }
    </script>
  </body>
</html>
)=====";
