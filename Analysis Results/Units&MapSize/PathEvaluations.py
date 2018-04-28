#import pdb
#pdb.set_trace()

import statistics

# Read and Open the files for processing
filename = input("Enter the Filename: ")
file = open(filename + ".txt", "r")
results = open(filename + "_results.txt", "w")

# Get the data for processing
requests = file.readline()
answers = file.readline()
requests_times = file.readline()
answers_times = file.readline()

# remove any white space
requests = requests.strip('\n')
answers = answers.strip('\n')
requests_times = requests_times.strip('\n')
answers_times = answers_times.strip('\n')

# CSV split it 
request_list = requests.split(',')
answers_list = answers.split(',')
requests_times_list = requests_times.split(',')
answers_times_list = answers_times.split(',')

# Remove and verify the order is correct
request_list.remove("unitPathRequestMade")
answers_list.remove("unitPathRequestProcessed")
requests_times_list.remove("unitPathRequestMade_TimeStamp")
answers_times_list.remove("unitPathRequestProcessed_TimeStamp")

# debug
#print(request_list[0])
#print(answers_list[0])
#print(requests_times_list[0])
#print(answers_times_list[0])

#input("Verify")

# Create a list to store the results
timeList = []

# Process the data
while(len(request_list) >= 1):

	# Pop the next path request to check
	path = request_list.pop(0)
	
	# debug
	#print(path)
	
	# debug
	#print(answers_list[0])
	
	# see if the request was responsed to in the remaining answers
	if(answers_list.count(path) > 0):
	
		# Find where it is in the list
		indexLocaiton = answers_list.index(path)
		
		# debug
		#print(indexLocaiton)
		
		answers_list.pop(indexLocaiton)
		
		# Get the corresponding time points 
		startTime = requests_times_list.pop(0)
		
		# debug
		#print(startTime)
		if(len(answers_times_list) > 0):
			endTime = answers_times_list.pop(indexLocaiton)
		else:
			results.write("Data Missmatch")
			break
		
		# debug
		#print(endTime)
		
		# Calculate the time between the start and finish and store it
		timeList.append(float(endTime) - float(startTime))
		
		# output the results 
		results.write(str(float(endTime) - float(startTime)) + ",")
		
		# debug
		#print(str(float(endTime) - float(startTime)) + ",")
		
		# debug
		#input("Verify")
		

		
# debug
#input("Verify")

# Calculate and store the final results	
timeSum = sum(timeList)
timeAverage = timeSum/len(timeList)
results.write("\nThe average path time is:")
results.write(str(timeAverage))
medianNum = statistics.median(timeList)
results.write("\nThe median path time is:")
results.write(str(medianNum))
results.write("\nThe longest path time is:")
results.write(str(max(timeList)))
	
	# debug
	#input("Verify")	

# close the files
results.close()
file.close()