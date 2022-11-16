# PlantMonitor

The plant monitor is used to monitor the current health condition of the plant, hoping that the plant will stay healthy and active.
Through the collection of temperature, humidity and other data, these data will be sent to the specified topic through the mqtt protocol over time, users can subscribe to the topic to obtain the relevant data.
Data is also transferred to the Raspberry PI database, which can be visualized using tools such as influxdb or grafana to facilitate better data analysis.
