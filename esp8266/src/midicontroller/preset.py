from .action import Action


class Preset:

    name = None
    actions = []

    def __init__(self, name, actions):
        self.load(name, actions)

    def load(self, name, actions):
        self.name = name
        del self.actions
        self.actions = []
        for action in actions:
            self.actions.append(Action(action["type"], **action["parameters"]))

    def pressed(self):
        for action in self.actions:
            action.do_action()
