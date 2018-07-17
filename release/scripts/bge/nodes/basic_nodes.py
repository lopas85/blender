import bge

class LogicNodeRoot(bge.types.LOG_Node):
	def start(self):
		pass

	def update(self):
		return self.triggers[0]

class LogicNodeBoolean(bge.types.LOG_Node):
	def start(self):
		pass

	def update(self):
		return self.triggers[0]

class LogicNodeBasicMotion(bge.types.LOG_Node):
	def start(self):
		pass

	def update(self):
		return self.triggers[0]
