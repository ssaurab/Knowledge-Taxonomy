This is an interactive CLI-based Taxonomy for a KB of the general, ALC-based, description logic. It can also generate .png files showing the taxonomy at any given moment using a python-based image generation of graphs.  

We solve the problem of taking in a set of concept definitions, which forms the Tbox, and use it to built the taxonomy. 
Later on, we take in the taxonomy and the Tbox along with the collection of given facts (which forms the Abox), and support the queries and edits as given below.

![The Assignment Structure](https://github.com/SaurabSirpurkar/KRR_Project/blob/main/Flow.png)


Upon input of an individual, we always place them in the Abox under the most specific concept of which they are instances of. 
Additionally, we support three kinds of queries which are:
	1. Get all individuals of a concept
	2. Get all concepts of an individual
	3. Say whether a given individual is an instance of a given concept
	
We also support addition of facts on the go that will update the place of individuals in the taxonomy, should individuals should become instances of more specific concepts.

We have a pre-loaded hospital taxonomy in our submission and this can be directly interacted from CLI after running make. To continue over this taxonomy, please understand the given ABox and TBox by going through facts.txt and conceptDefinitions.txt respectively.

![The Hospital Taxonomy](https://github.com/SaurabSirpurkar/KRR_Project/blob/main/Taxonomy.png)


	
