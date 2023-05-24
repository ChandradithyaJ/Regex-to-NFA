import visuals

def main():
	regex = input("Enter the regular expression: ")
	visuals.create_digraph(regex)

if __name__ == '__main__':
	main()