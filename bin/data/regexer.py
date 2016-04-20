import re

with open("allI.txt") as f, open('allIFix.txt', 'w') as out:
	for line in f:
		line = line.replace("end", "end\n\n")
		out.write(line)
	print "done"

# with open("allO.txt") as f:
# 	for line in f:
# 		line = line.replace("end", "end\n\n")
# 		print line