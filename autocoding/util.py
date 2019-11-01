def generateComment(comment, length, indentation = ""):
    out = ""
    for commentChunk in [comment[i : i + length] for i in range(0, len(comment), length)]:
        out += indentation + "// " + commentChunk + "\n"
    return out

def getDataReference(element, root):
    for parent in root.iter():
        # Check to make sure parent is actually a parent of element
        if element in parent:
            return getDataReference(parent, root) + " -> " + element.attrib["id"]
    return root.attrib["id"]