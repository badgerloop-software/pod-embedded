# Generates comments with the specified indentation and wrapping-length
def generateComment(comment, length, indentation = ""):
    out = ""
    for commentChunk in [comment[i : i + length] for i in range(0, len(comment), length)]:
        out += indentation + "// " + commentChunk + "\n"
    return out

# Generates C++ pointer data references from an XML element
def getDataReference(element, root):
    for parent in root.iter():
        # Check to make sure parent is actually a parent of element
        if element in parent:
            return getDataReference(parent, root) + " -> " + element.attrib["id"]
    return root.attrib["id"] + ""

def getCPPReadable(reference, type, inArray = False):
    if inArray:
        reference = reference + "[i]"
    if type == "bool":
        return "(" + reference + ' ? "True" : "False" )'
    elif type == "float":
        return 'std::to_string(' + reference + ').append("f")'
    else:
        return "std::to_string(" + reference + ")"