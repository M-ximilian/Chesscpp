from berserk import Client, TokenSession

Token = "lip_Vk5q5MDt7dQHt4hRen1N"


class Lichess:
    def __init__(self):
        self.client = Client(session=TokenSession(Token))

    def get_session(self):
        self.client = Client(session=TokenSession(Token))

    def get_game(self):
        game_started = False
        moves = [""]
        last_move = None

        while True:
            print("pending")
            self.get_session()
            for event in self.client.bots.stream_incoming_events():
                if event['type'] == 'gameStart':
                    game_started = True

                    while game_started is True:
                        self.get_session()
                        if last_move != self.client.games.get_ongoing()[0]['lastMove']:
                            moves.append(self.client.games.get_ongoing()[0]['lastMove'])
                            last_move = self.client.games.get_ongoing()[0]['lastMove']
                            print(last_move)

    def write_moves(self):
        pass

    def read_moves(self):
        pass


if __name__ == '__main__':
    lichess = Lichess()
    lichess.get_game()
