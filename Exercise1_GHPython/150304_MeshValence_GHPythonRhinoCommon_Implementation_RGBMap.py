"""
Color a mesh based on vertex valence.
"""

import Rhino as rc

def getVertexValences(mesh):
    
    """ Get vertex valences for a mesh """
    
    valences = []
    for i in range(Mesh.Vertices.Count):
        
        # Get list of neighbouring vertices for vertex i
        tId = Mesh.TopologyVertices.TopologyVertexIndex(i)
        neighbourIds = Mesh.TopologyVertices.ConnectedTopologyVertices(tId)
        
        # Calculate the valence
        v = len(neighbourIds)
        valences.append(v)
        
    return valences

def remapValues(values,targetMin,targetMax):
    
    """ Remap a list of values into a target domain """
    
    # Check that different values exist
    if len(set(values)) > 1:
        
        remappedValues = []
        
        # Get sourceDomain min and max
        srcMin = min(values)
        srcMax = max(values)
        
        # Iterate the values and remap them
        for v in values:
            rv = ((v-srcMin)/(srcMax-srcMin))*(targetMax-targetMin)+targetMin
            remappedValues.append(rv)
            
        return remappedValues
        
    # Else return targetMax for each value
    else:
        return [targetMin]*len(values)

def valueToRGBColor(v):
    
    """ Scale the red/green/blue color values """
    
    r = (1.0 * v) / 100
    g = (1.0 * (100 - v)) / 100
    b = (1.0 * (1000 - v)) / 100
    
    return r,g,b

def mapValueListAsColors(values,min,max):
    
    """ Make a list of RGB colors where the values are mapped onto a
    min-max hue domain """
    
    colors = []
    remappedValues = remapValues(values,min,max)
    for v in remappedValues:
        r,g,b = valueToRGBColor(v)
        c = rc.Display.ColorXYZ(r,g,b)
        colors.append(c)
        
    return colors

def colorMesh(mesh,colors):
    
    """ Color mesh vertices by list of colors """
    
    for i,c in enumerate(colors):
        mesh.VertexColors.SetColor(i,c)


# Function calls
valences = getVertexValences(Mesh)
colors = mapValueListAsColors(valences,0,100) # Range of domain should now be 0-100 to work with 
VMesh = Mesh
colorMesh(VMesh,colors)