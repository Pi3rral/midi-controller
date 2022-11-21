class Env:

    _env = {}

    @classmethod
    def set(cls, key: str, value: str):
        cls._env[key] = value

    @classmethod
    def get(cls, key: str, default=None):
        return cls._env.get(key, default=default)
