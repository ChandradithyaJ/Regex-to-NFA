import visuals

def main():
    regex = input("Enter the regular expression: ")
    visuals.create_digraph(regex)

    """
     Accepting states = misty rose
     {States} - {Accepting States} = sky blue
    """

if __name__ == '__main__':
    main()