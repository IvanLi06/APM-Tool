#!/bin/bash

LOG_DIR="./logs"         # directory for storing logs
IP_ADDRESS="8.8.8.8"     # Can change IP
FILE_PATH="./"            # Current directory containing APM tool and execuatble processes
PROCESSES=("bandwidth_hog" "bandwidth_hog_burst" "cpu_hog" "disk_hog" "memory_hog" "memory_hog_leak")  

#Create the logs directory
mkdir -p "$LOG_DIR"

echo "Starting APM"

# Track PIDs for all the processes
declare -A PIDS  # Array to store the process name and PID
START_TIME=$(date +%s)  # Record the start time

# starts processes and records their PIDS
start_process() {
    for process in "${PROCESSES[@]}"; do
        echo "Starting process: $process"
        nohup $FILE_PATH/$process $IP_ADDRESS > /dev/null 2>&1 & #start the processes in the background
        PID=$!  # Get the PID of current process
        PIDS["$process"]=$PID  # Store the pid in the array created before
    done
}

#Run all process with the function
start_process

# Cleanup function to kill all the processes
cleanup() {
    for process in "${PROCESSES[@]}"; do
        PID=${PIDS["$process"]}
        if ps -p $PID > /dev/null 2>&1; then
            echo "Killing process $PID"
            kill $PID
        fi
    done
    echo "APM stopped."
    exit 0
}

#Cleanup (Ctrl+C)
trap cleanup SIGINT

# Track the last recorded time to make sure all the intervals are 5 seconds(was not 5 seconds before implementing this)
last_recorded_time=$START_TIME

# Loop to monitor stats for all the processes
send_to_csv() {
current_time=0
while true; do
    #current_time= #$(date +%s)
    # Check if 5 seconds have passed
    #if (( (current_time - last_recorded_time) >= 5 )); then
        # Loop through each process and log the stats
        for process in "${PROCESSES[@]}"; do
	    if [ ! -f "$LOG_DIR/${process}_metrics.csv" ]; then 
		echo "Seconds, CPU, Memory" > "$LOG_DIR/${process}_metrics.csv" 
	    fi
            #Get the PID that was recorded before
            PID=${PIDS["$process"]}
            if [ -n "$PID" ] && ps -p $PID > /dev/null 2>&1; then
                # Collect all the process stats
                STATS=$(ps -p $PID -o %cpu,%mem --no-headers)
                CPU_USAGE=$(echo "$STATS" | awk '{print $1}')
                MEM_USAGE=$(echo "$STATS" | awk '{print $2}')
                # Calculate the elapsed time
                ELAPSED_TIME=$(($current_time - $START_TIME))
                # Send all stats to their corresponding csv's
                echo "$current_time, $CPU_USAGE, $MEM_USAGE" >> "$LOG_DIR/${process}_metrics.csv"
                echo "$process CPU: $CPU_USAGE% | MEM: $MEM_USAGE KB | PID: $PID | Seconds: $current_time seconds"
            else
                echo "$process is not running, restarting"
		# Restart the process if it did not run
                nohup $FILE_PATH/$process $IP_ADDRESS > /dev/null 2>&1 &
                PID=$!  # Get the PID of the new process
                PIDS["$process"]=$PID  # Update the PID for this process
	   fi
	done
        # Update the recorded time
        #last_recorded_time=$current_time
	((current_time+=5))
	sleep 5
done

}


system_metrics(){
  OUTPUT=system_metrics.csv
  seconds=0
  echo "Seconds,RX Data Rate (kB/s),TX Data Rate (kB/s),Disk Writes (kB/s),Available Disk Capacity (MB)" > $OUTPUT
  while true; do
    write_usage=$(iostat -d -k | awk '$1 == "sda" {print $4}')
    storage_usage=$(df -m / | awk 'NR==2 {print $4}')
    RX_usage=$(ifstat ens192 | grep "^ens192" | awk '{print $6 }')
    TX_usage=$(ifstat ens192 | grep "^ens192" | awk '{print $8 }')
    echo "Collecting System Metrics" 
    echo "$seconds, $RX_usage, $TX_usage,  $write_usage, $storage_usage" >> $OUTPUT
    ((seconds+=5))
    sleep 5
done

}

system_metrics & send_to_csv

