# Social Choice Theory - A Toolbox

The purpose is the following: to create a library of Axiomatic Social Choice Theory canonical models, e.g., Arrow's impossibility theorem. The library is intended to help, or aid, students that are being introduced to Social Choice Theory.
They will be able to follow, step-by-step, the theorems implemented here.

# About the properties

ID and Preferences may be obvious. In SCT, for a set X of alternatives and an indexed set S of agents, ID represents any agent s<sub> i </sub> of S, while Preferences represents s<sub> i </sub>'s ranking of every x <span>&#8714;</span> X, i.e., the property Preferences is already ordered accordingly to s<sub> i </sub>'s respective ranking of the mentioned options.

# In progress

Implement Indifference relation: done.

Implement Condorcert Paradox - working on it

Implement Arrow's Impossibility Theorem - after Condorcet Par.

Implement Gibbard-Sattertwaite Theorem - after Arrow.

Preferences will be externaly given, but agents will be able to modify them.

The data structure used will be a Bidimensional Matrix, composed by three elements, i.e., Agents, Alternatives/Options, and a Value which will be representing the ranking of an alternative/option in an agent ordering.

<ul type="circle">
  
<p><li><strong>How will indifference be represented?</strong></li>

&emsp;For any two x and y preferences in an agent's ordering, if x and y have both the same value, then one may say that the agent is indifferent between both of them, and, trivially, for any two options a and b, if the values of a and b are different, the option with a higher value will come, obviously, first in the ordering, being, thus, preferred by the agent.<\p>

<p><li><strong>How will the agents have access to their preferences?</strong></li>

&emsp;Each agent will have access to one, and only one, row of the matrix. This will prevent having access, at least at the beginning, to other agent's preferences. <\p>

<p><li><strong>Why are preferences external?</strong></li>
 
&emsp;To be simple: it is easier to implement and it saves memory.<\p>

</ul>

# For later:

Add to this README file an visual representation of the Preferences Matrix Structure.

The plan is to add canonical models, for example, Arrow's Impossíbility Theorem, when I start studying, for real, Social Choice Theory. I will be using Wulf Gaertner's A Primer On Social Choice Theory( 2006 ) as textbook, and every program of model will be based on the books presentation of the respective model.
