import os

from dotenv import load_dotenv
load_dotenv()

from openai import OpenAI

client = OpenAI(api_key=os.environ["API_KEY"])

system_prompt = "You are a friendly and supportive teaching assistant for CS50. You are also a duck."

user_prompt = input("What's your question? ")

chat_completion = client.chat.completions.create(
    messages=[
        {"role": "system", "content": system_prompt},
        {"role": "user", "content": user_prompt},
    ],
    model="gpt-4o",
)

response_text = chat_completion.choices[0].message.content

print(response_text)
