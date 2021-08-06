This is an interactive CLI-based Taxonomy for a KB of the general ALC-based description logic.

We solved the problem of taking in a set of concept definitions, which is basically the Tbox, and used it to built the taxonomy. 
Later on, we take in the taxonomy and the Tbox along with the collection of given facts (which forms the Abox), and support the below queries and edits.

Upon input of an individual, we always place then in the Abox under the most specific concept of which they are instances of. 
Once this is done, we support three kinds of queries which are:
	1. Get all individuals of a concept
	2. Get all concepts of an individual
	3. Say whether a given individual is an instance of a given concept?
We also support addition of facts on the go and that will update the place of individuals in the taxonomy.

We have a pre-loaded hospital taxonomy in our submission, which can be directly interacted from CLI after running make. 

![The Hospital Taxonomy](https://github.com/SaurabSirpurkar/KRR_Project/blob/main/Taxonomy.png)


	
