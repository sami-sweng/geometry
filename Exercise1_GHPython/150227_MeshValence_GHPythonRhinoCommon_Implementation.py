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

def mapValueListAsColors(values,min,max):
    
    """ Make a list of HSL colors where the values are mapped onto a
    min-max hue domain. Meaning that low values will be red, medium
    values green and large values blue """
    
    colors = []
    remappedValues = remapValues(values,min,max)
    for v in remappedValues:
        c = rc.Display.ColorHSL(v,1.0,0.5).ToArgbColor()
        colors.append(c)
        
    return colors

def colorMesh(mesh,colors):
    
    """ Color mesh vertices by list of colors """
    
    for i,c in enumerate(colors):
        mesh.VertexColors.SetColor(i,c)


# Function calls
valences = getVertexValences(Mesh)
colors = mapValueListAsColors(valences,0.0,0.7) # 0.0 - 0.7 is the classic red to blue hue gradient
VMesh = Mesh
colorMesh(VMesh,colors)