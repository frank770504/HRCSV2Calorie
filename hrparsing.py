import os
import sys
import csv

class HeartRate:
	rate = 0
	timestamp = 0
	timediff = 0

class InitData:
	age = 0
	weight = 0
	location = 0
	gender = 0

class Speed:
	speed = 0
	timestamp = 0;


def csv_parsing(name):
	hr_list = [];
	user = InitData();
	with open(name, 'rb') as f:
		temp = csv.reader(f, delimiter = ';');
		for row in temp:
			item = row[2].split(':');
			#print item[0];
			if item[0] == 'Heart rate':
				hr = HeartRate();
				hr.timestamp = int(row[0]);
				hr.rate = int(row[3]);
				if not hr_list:
					hr.timediff = 0;
				else:
					hr.timediff = hr.timestamp - hr_list[-1].timestamp;
				hr_list.append(hr);
			elif item[0] == 'Age':
				user.age = int(item[1]);
			elif item[0] == 'Weight':
				user.weight = float(item[1]);
			elif item[0] == 'Gender':
				user.gender = str(item[1]).replace(" ","");
#			elif item[0] == 'Speed':
#			else
	out = []
	out.append(hr_list);
	out.append(user);
	return out

def do_cal(hr_lst, user):
	cal = 0;
	for hrx in hr_lst:
		time = float(hrx.timediff) / 60000;
		if user.gender == 'Male':
			cal += ((-55.0969 +(0.6309*hrx.rate)+(0.1988*user.weight)+(0.2017*user.age))/4.184)*time;
		elif user.gender == 'Female':
			cal += ((-20.4022 +(0.4472*hrx.rate)+(0.1263*user.weight)+(0.074*user.age))/4.184)*time;
		else:
			cal = -1.414;
	return cal

################################
#### ===== main script ==== ####
################################

#get files
f_names = sys.argv;

f_num = len(sys.argv);

f_names = f_names[1:f_num];

book = [];

class page:
	name = 0
	cal = 0

#for each file
for name in f_names:
	out = csv_parsing(name);
	hr_list = out[0];
	user = out[1];
	cal = do_cal(hr_list, user);
	p = page();
	p.name = name;
	p.cal = cal;
	book.append(p);

# print user data
print 'User Statue'
print '|-Age:    %d' % user.age
print '|-Weignt: %d kg' % int(user.weight)
print '|-Gender: %s' % user.gender
print ''

print 'Calorie'
# print cal
for p in book:
	cell = [p.name, p.cal];
	print '{0}: {1} kcal'.format(*cell)
