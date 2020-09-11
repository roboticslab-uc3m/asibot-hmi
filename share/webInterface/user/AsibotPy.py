import yarp

VOCAB_STAT = yarp.encode('stat')
VOCAB_INV = yarp.encode('inv')
VOCAB_MOVJ = yarp.encode('movj')
VOCAB_MOVL = yarp.encode('movl')
VOCAB_MY_STOP = yarp.encode('stop')
VOCAB_WAIT = yarp.encode('wait')

class CartesianClient:
    p = yarp.Port()

    def open(self, prefix):
        clientPort = prefix
        clientPort += '/cartesianClient/rpc:o'
        self.p.open(clientPort)
        serverPort = prefix
        serverPort += '/cartesianServer/rpc:i'
        self.p.addOutput(serverPort)
        return True

    def close(self):
        self.p.interrupt()
        self.p.close()
        return True

    def stat(self, res):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_STAT)
        self.p.write(miOutput, miInput)
        data = miInput.get(0).asList()
        del res[:]
        for elem in range(0,data.size()):
            res.append(data.get(elem).asFloat64())
        return True

    def stop(self):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_MY_STOP)
        self.p.write(miOutput, miInput)
        return True

    def inv(self, xd, res):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_INV)
        dBottle = yarp.Bottle()
        dBottle = miOutput.addList()
        for elem in range(0,len(xd)):
            dBottle.addFloat64(xd[elem])
        self.p.write(miOutput, miInput)
        data = miInput.get(0).asList()
        del res[:]
        for elem in range(0,data.size()):
            res.append(data.get(elem).asFloat64())
        return True

    def movj(self, xd):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_MOVJ)
        dBottle = yarp.Bottle()
        dBottle = miOutput.addList()
        for elem in range(0,len(xd)):
            dBottle.addFloat64(xd[elem])
        self.p.write(miOutput, miInput)
        return True

    def movl(self, xd):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_MOVL)
        dBottle = yarp.Bottle()
        dBottle = miOutput.addList()
        for elem in range(0,len(xd)):
            dBottle.addFloat64(xd[elem])
        self.p.write(miOutput, miInput)
        return True

    def wait(self):
        miOutput = yarp.Bottle()
        miInput = yarp.Bottle()
        miOutput.clear()
        miOutput.addVocab(VOCAB_WAIT)
        self.p.write(miOutput, miInput)
        return True

yarp.Network.init()

if yarp.Network.checkNetwork() != True:
    print "[error] Please try running yarp server"
    quit()
