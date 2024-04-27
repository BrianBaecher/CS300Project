# CS300Project

> What was the problem you were solving in the projects for this course?

To implement a simple program that reads a text file, stores the inputted data in a data structure of our choosing, and allow for the user to browse the stored information.

> How did you approach the problem?

The coursework involved creating pseudocode for versions of the program that use differing data structures to facilitate the storage and accessing of the data. We sketched out three versions of the program; one using vectors, one using a hash table, and one using a binary search tree. The process of creating pseudocode/outlines for each version of the program was very helpful in ultimately deciding which data structure I felt would be most appropriate for the final version.

> How did you overcome any roadblocks you encountered while going through the activities or project?

Thanks to the aforementioned psuedocode, writing the program was a fairly smooth process. I managed to account for most of the issues I'd have to account for within the outline I created. A minor issue I ran into was the means by which the Course objects could be compared; this issue was pretty quickly resolved after doing a bit of reading through C++ documentation, after which I created a small helper function that is used to facilitate the comparison of the Class type - I had initially written it as a lambda function but the seperate definition enhances the readability of the program's code, to some degree.

> How has your work on this project expanded your approach to designing software and developing programs?

It has highlighted the importance of having a well-established plan before diving into actually writing a program. Having a detailed outline proved immensely beneficial, especially when it came to designing the Course class. Often times when working with classes, I'll find myself revisiting the class definition fairly often to add/edit methods and member variables - whereas in this exercise I was able to define the class at the outset and was confident all the necessary methods/members were already available, which streamlined the process. So I've developed a greater appreciation for the planning stages involved in development.

> How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

This project has also highlighted the importance and utility of well-commented code. Generally, I'm writing code that no one but myself will ever look at, so I don't always go the extra mile and document files appropriately. I've come to realize that providing detailed comments and definitions isn't solely for the benefit of others, but is also an enormous help for the author as well.
