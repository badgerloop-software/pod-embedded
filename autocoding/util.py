# Generates comments with the specified indentation and wrapping-length
def generateComment(comment, length = 100, indentation = ""):
    out = ""
    for commentChunk in [comment[i : i + length] for i in range(0, len(comment), length)]:
        out += indentation + "// " + commentChunk + "\n"
    return out

# Generates C++ pointer data references from an XML element
def getDataReference(element, root):
    for parent in root.iter():
        # Check to make sure parent is actually a parent of element
        if element in parent:
            return getDataReference(parent, root) + "->" + element.attrib["id"]
    return root.attrib["id"]

def capitalize(string):
    return string.capitalize()[0] + string[1:]

def getSemaphoreReference(struct, root):
    return getDataReference(struct, root) + "->" + struct.attrib["id"] + "Semaphore"

def getGetReference(struct, field):
    return "get" + capitalize(struct.attrib["id"]) + capitalize(field.attrib["id"])

def getSetReference(struct, field):
    return "set" + capitalize(struct.attrib["id"]) + capitalize(field.attrib["id"])